#include "PartitionTableReader.hpp"
#include "Utils.hpp"

#include <whb/log.h>

PartitionTableReader::PartitionTableReader(const DiscInterface *discInterface)
: m_ready(false)
, m_gptPresent(false)
{
    if (!discInterface->readSectors(0, 1, &m_mbr)) {
        WHBLogPrintf("PartitionTableReader: Error while reading first sector");
        return;
    }

    WHBLogPrintf("PartitionTableReader: Succesfully read first sector");
    if (m_mbr.signature != 0x55AA && m_mbr.signature != 0x55AB) {
        WHBLogPrintf("PartitionTableReader: Invalid MBR signature found!");
        return;
    }

    // Extract partitions from MBR
    m_mbrPartitions = std::vector<MBR_PARTITION>(std::begin(m_mbr.partitions), std::end(m_mbr.partitions));

    // We want little endian
    for (MBR_PARTITION& partition : m_mbrPartitions) {
        correctMBRPartitionEndianness(&partition);
    }

    // Check if it's a protective MBR
    if (m_mbrPartitions[0].partitionType == 0xEE) {
        // TODO: Check the GPT header and discover partitions
        WHBLogPrintf("Protective MBR found!");
        m_gptPresent = true;
    } else {
        // This is not a protective MBR, so we attempt to discover any extended partition
        for (const MBR_PARTITION& partition : m_mbrPartitions) {
            discoverExtendedPartition(discInterface, partition, &m_ebrPartitions);
        }

        WHBLogPrintf("Discovered %u EBRs", m_ebrPartitions.size());
    }

    m_ready = true;
}

void PartitionTableReader::discoverExtendedPartition(const DiscInterface *discInterface,
                                                     const MBR_PARTITION &extendedPartitionEntry,
                                                     std::vector<EBR_PARTITION> *logicalPartitions) {
    // Check that the entry partition type is extended partition
    if (extendedPartitionEntry.partitionType != 0x05 && extendedPartitionEntry.partitionType != 0x0F && extendedPartitionEntry.partitionType != 0x85)
        return;

    MBR_PARTITION nextEBR;
    uint32_t sectorToCheck = extendedPartitionEntry.startingLBA;

    do {
        MASTER_BOOT_RECORD ebr;

        // Sanity checks
        if (!discInterface->readSectors(sectorToCheck, 1, &ebr))
            break;

        if (ebr.signature != 0x55AA && ebr.signature != 0x55AB)
            break;

        // We need to keep track of this because an EBR entry's starting LBA is relative to the EBR starting LBA
        EBR_PARTITION currentLogicalPartition;
        currentLogicalPartition.ebrLBA = sectorToCheck;

        // Store first two entries
        correctMBRPartitionEndianness(&ebr.partitions[0]);
        currentLogicalPartition.partition = ebr.partitions[0];
        logicalPartitions->push_back(currentLogicalPartition);

        correctMBRPartitionEndianness(&ebr.partitions[1]);
        nextEBR = ebr.partitions[1];
        sectorToCheck = nextEBR.startingLBA + extendedPartitionEntry.startingLBA;
    } while (nextEBR.startingLBA != 0);
}

void PartitionTableReader::correctMBRPartitionEndianness(PartitionTableReader::MBR_PARTITION *partition) {
    partition->startingLBA = Utils::swapEndian32(partition->startingLBA);
    partition->totalBlocks = Utils::swapEndian32(partition->totalBlocks);
}

bool PartitionTableReader::hasGPT() {
    return m_gptPresent;
}

bool PartitionTableReader::isReady() const {
    return m_ready;
}

const PartitionTableReader::MASTER_BOOT_RECORD &PartitionTableReader::getMbr() const {
    return m_mbr;
}

const std::vector<PartitionTableReader::MBR_PARTITION> &PartitionTableReader::getMbrPartitions() const {
    return m_mbrPartitions;
}

const std::vector<PartitionTableReader::EBR_PARTITION> &PartitionTableReader::getEbrPartitions() const {
    return m_ebrPartitions;
}
