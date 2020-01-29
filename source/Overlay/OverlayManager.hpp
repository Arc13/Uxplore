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

#ifndef UXPLORE_CPP_OVERLAYMANAGER_HPP
#define UXPLORE_CPP_OVERLAYMANAGER_HPP

#include <memory>
#include <vector>

#include "ManagedOverlay.h"

class OverlayManager {
public:
    void processEvent(const SDL_Event &event) {
        if (!m_overlayStack.empty()) {
            m_overlayStack.back()->processEvent(event);
        }
    }

    void update(const float &delta) {
        for (auto &m_overlay : m_overlayStack) {
            m_overlay->update(delta);
        }
    }

    void renderPrimary(SDL_Renderer &renderer, TTF_Font &font) {
        for (auto &m_overlay : m_overlayStack) {
            m_overlay->renderPrimary(renderer, font);
        }
    }

    void renderSecondary(SDL_Renderer &renderer, TTF_Font &font) {
        for (auto &m_overlay : m_overlayStack) {
            m_overlay->renderSecondary(renderer, font);
        }
    }

    void pushOverlay(std::unique_ptr<ManagedOverlay> overlay) {
        m_overlayStack.push_back(std::move(overlay));
    }

    void popOverlay() {
        m_overlayStack.pop_back();
    }
private:
    std::vector<std::unique_ptr<ManagedOverlay>> m_overlayStack;
};


#endif //UXPLORE_CPP_OVERLAYMANAGER_HPP