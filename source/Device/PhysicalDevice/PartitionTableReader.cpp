#include "PartitionTableReader.hpp"
#include "Utils.hpp"

#include <cstring>
#include <whb/log.h>

#define MBR_SIG 0x1FE

PartitionTableReader::PartitionTableReader(const DiscInterface* discInterface)
: m_returnCode(-1)
, m_gpt(false)
{
    uint8_t mbrData[512];
    if (discInterface->readSectors(0, 1, &mbrData)) {
        uint16_t signature = ((uint16_t)mbrData[MBR_SIG] << 8) | mbrData[MBR_SIG + 0x01];

        WHBLogPrintf("PartitionTableReader: Succesfully read first sector");
        if (signature != 0x55AA && signature != 0x55AB) {
            WHBLogPrintf("PartitionTableReader: Invalid MBR signature found!");
            return;
        }

        // Extract partitions from MBR
        for (unsigned i = 0; i < 4; ++i) {
            m_mbrPartitions.push_back(getMBRPartitionFromData(&mbrData[0x1BE + (sizeof(MBR_partition) * i)]));
        }

        // Check if it's a protective MBR
        if (m_mbrPartitions[0].partitionType == 0xEE) {
            // TODO: Check the GPT header and discover partitions
            WHBLogPrintf("Protective MBR found!");
            m_gpt = true;
        } else {
            // Not a protective MBR, discover EBRs instead
            std::vector<EBR_partition> discoveredEBRs;
            discoverEBRPartitions(discInterface, m_mbrPartitions, &discoveredEBRs);

            WHBLogPrintf("Discovered %u EBRs", discoveredEBRs.size());
        }
    } else {
        WHBLogPrintf("PartitionTableReader: Error while reading first sector");
    }
}

void PartitionTableReader::discoverEBRPartitions(const DiscInterface *discInterface,
                                                 const std::vector<MBR_partition> &mbrPartitions,
                                                 std::vector<EBR_partition> *ebrPartitions) {
    for (const MBR_partition & partition : mbrPartitions) {
        if (partition.partitionType == 0x05 || partition.partitionType == 0x0F || partition.partitionType == 0x85) {
            // Partition is an EBR
            EBR_partition currentEBR, nextEBR;
            uint8_t ebrData[512];
            uint32_t sectorToCheck = partition.firstSector;

            // Only fill this information in currentEBR because nextEBR won't be kept
            currentEBR.extendedPartitionStartSector = partition.firstSector;

            do {
                if (!discInterface->readSectors(sectorToCheck, 1, &ebrData))
                    break;

                uint16_t signature = ((uint16_t)ebrData[MBR_SIG] << 8) | ebrData[MBR_SIG + 0x01];
                if (signature != 0x55AA && signature != 0x55AB)
                    break;

                currentEBR.partition = getMBRPartitionFromData(&ebrData[0x1BE]);
                ebrPartitions->push_back(currentEBR);

                nextEBR.partition = getMBRPartitionFromData(&ebrData[0x1CE]);
                sectorToCheck = nextEBR.partition.firstSector + partition.firstSector;
            } while (nextEBR.partition.firstSector != 0);
        }
    }
}

PartitionTableReader::MBR_partition PartitionTableReader::getMBRPartitionFromData(uint8_t *data) {
    MBR_partition partition;
    memcpy(&partition, data, sizeof(MBR_partition));

    partition.firstSector = Utils::swapEndian32(partition.firstSector);
    partition.totalSectors = Utils::swapEndian32(partition.totalSectors);

    return partition;
}

bool PartitionTableReader::hasGPT() {
    return m_gpt;
}