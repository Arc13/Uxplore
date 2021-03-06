/*
    Uxplore
    Copyright (C) 2019-2021, Kuruyia

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

#include "../Macros.h"
#include "Dialog.h"

#include <utility>
#include "OverlayManager.hpp"

Dialog::Dialog(OverlayManager& manager, std::string message)
        : ManagedOverlay(manager)
        , m_message(std::move(message))
{

}

void Dialog::update(UNUSED_PARAM const float& delta)
{

}

void Dialog::processEvent(const Event& event)
{
    // Remove the Dialog if the B button was pressed
    if (event.m_eventType == EventType::EVENT_SDL && event.m_eventData.m_sdlEvent.type == SDL_JOYBUTTONUP &&
        event.m_eventData.m_sdlEvent.jbutton.button == 1)
    {
        m_overlayManager.popOverlay();
    }
}

void Dialog::renderPrimary(SDL_Renderer& renderer, UNUSED_PARAM TTF_Font& font)
{
    SDL_SetRenderDrawColor(&renderer, 0, 0, 0, 0xDF);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 1280;
    rect.h = 720;
    SDL_RenderFillRect(&renderer, &rect);
}

void Dialog::renderSecondary(UNUSED_PARAM SDL_Renderer& renderer, UNUSED_PARAM TTF_Font& font)
{

}
