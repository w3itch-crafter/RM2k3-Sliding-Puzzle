/////////////////////////////////////////////////////////////////////////////
// This file is part of EasyRPG Player.
// 
// EasyRPG Player is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// EasyRPG Player is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "scene_equip.h"
#include "game_party.h"
#include "game_system.h"
#include "graphics.h"
#include "input.h"
#include "player.h"
#include "scene_menu.h"

////////////////////////////////////////////////////////////
Scene_Equip::Scene_Equip(int actor_index, int equip_index) :
	actor_index(actor_index),
	equip_index(equip_index) {
	type = Scene::Equip;

	// Debug Test code to add items
	for (int i = 1; i < 30; ++i) {
		Game_Party::GainItem(i, i + 10);
	}
}

////////////////////////////////////////////////////////////
Scene_Equip::~Scene_Equip() {
	delete help_window;
	delete equip_window;
	delete status_window;
	for (int i = 0; i < 5; ++i) {
		delete item_windows[i];
	}
}

////////////////////////////////////////////////////////////
void Scene_Equip::Start() {
	Game_Actor* actor = Game_Party::GetActors()[actor_index];

	// Create the windows
	help_window = new Window_Help();
	status_window = new Window_EquipStatus(actor->GetId());
	equip_window = new Window_Equip(actor->GetId());

	equip_window->SetIndex(equip_index); 

	for (int i = 0; i < 5; ++i) {
		item_windows.push_back(new Window_EquipItem(actor->GetId(),
			(Window_EquipItem::EquipType)i));
	}

	// Assign the help windows
	equip_window->SetHelpWindow(help_window);
	for (size_t i = 0; i < item_windows.size(); ++i) {
		item_windows[i]->SetHelpWindow(help_window);
	}
}

////////////////////////////////////////////////////////////
void Scene_Equip::Update() {
	help_window->Update();

	UpdateEquipWindow();
	UpdateStatusWindow();
	UpdateItemWindows();

	if (equip_window->GetActive()) {
		UpdateEquipSelection();
	} else if (item_window->GetActive()) {
		UpdateItemSelection();
	}
}
////////////////////////////////////////////////////////////
void Scene_Equip::UpdateItemWindows() {
	for (uint i = 0; i < item_windows.size(); ++i) {
		item_windows[i]->SetVisible((uint)equip_window->GetIndex() == i);
		item_windows[i]->Update();
	}

	item_window = item_windows[equip_window->GetIndex()];
}

////////////////////////////////////////////////////////////
void Scene_Equip::UpdateEquipWindow() {
	equip_window->Update();
}

////////////////////////////////////////////////////////////
void Scene_Equip::UpdateStatusWindow() {
	if (equip_window->GetActive()) {
		status_window->ClearParameters();
	} else if (item_window->GetActive()) {
		// ToDo
	}

	status_window->Update();
}

////////////////////////////////////////////////////////////
void Scene_Equip::UpdateEquipSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		Scene::instance = new Scene_Menu(2); // Select Equipment
	} else if (Input::IsTriggered(Input::DECISION)) {
		Game_System::SePlay(Data::system.decision_se);
		equip_window->SetActive(false);
		item_window->SetActive(true);
		item_window->SetIndex(0);
	}
}

////////////////////////////////////////////////////////////
void Scene_Equip::UpdateItemSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		equip_window->SetActive(true);
		item_window->SetActive(false);
		item_window->SetIndex(-1);
	} else if (Input::IsTriggered(Input::DECISION)) {
		Game_System::SePlay(Data::system.decision_se);
		equip_window->SetActive(true);
		item_window->SetActive(false);
		item_window->SetIndex(-1);

		equip_window->Refresh();

		for (size_t i = 0; i < item_windows.size(); ++i) {
			item_windows[i]->Refresh();
		}
	}
}
