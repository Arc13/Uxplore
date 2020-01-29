/*
    Uxplore
    Copyright (C) 2019-2020, Kuruyia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UXPLORE_CPP_MANAGEDOVERLAY_H
#define UXPLORE_CPP_MANAGEDOVERLAY_H

#include <memory>

#include "Overlay.h"

class OverlayManager;

class ManagedOverlay : public Overlay {
public:
    explicit ManagedOverlay(OverlayManager &manager);
    ~ManagedOverlay() override = default;

protected:
    void pushOverlay(std::unique_ptr<ManagedOverlay> overlay);
    void popOverlay();

private:
    OverlayManager &m_manager;
};


#endif //UXPLORE_CPP_MANAGEDOVERLAY_H
