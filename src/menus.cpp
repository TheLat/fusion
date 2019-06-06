#include "menus.h"

mutex mutex2;

void menu::create_menu(string file, string choice, string text_override, string followup_override) {
	image im;
	box b;
	text t;
	boxes.clear();
	arrowboxes.clear();
	images.clear();
	raw.clear();
	followup.clear();
	ifstream f(string("../resources/menus/") + file + string(".dat"));
	string line, temp1, temp2;
	while (f.is_open()) {
		while (std::getline(f, line)) {
			while (line.find("{NEWLINE}") != -1) {
				int i;
				i = line.find("{NEWLINE}");
				line.erase(i, string("{NEWLINE}").length());
				line.insert(i, string("\n"));
			}
			temp1 = line;
			temp2 = line;
			if (temp1.find(":") != -1) {
				temp1.erase(temp1.find(":"), temp1.length());
				temp2.erase(0, temp2.find(":") + 1);
				if (temp1 == "TYPE") {
					type = temp2;
					if (type == "ALERT")
						etype = ALERT;
					if (type == "SELECT")
						etype = SELECT;
					if (type == "AUTO_FOLLOWUP")
						etype = AUTO_FOLLOWUP;
					if (type == "SCROLL")
						etype = SCROLL;
				}
				if (temp1 == "AUTO_FOLLOW_UP") {
					done = !(stoi(temp2) == 0);
				}
				if (temp1 == "ALWAYS_CANCEL") {
					always_cancel = !(stoi(temp2) == 0);
				}
				else if (temp1 == "COLUMNS") {
					columns = stoi(temp2);
				}
				else if (temp1 == "CANCEL_OPTION") {
					cancel_option = stoi(temp2);
				}
				else if (temp1 == "CURSOR_OFFSET_X") {
					cursor_offset_x = stof(temp2);
				}
				else if (temp1 == "CURSOR_OFFSET_Y") {
					cursor_offset_y = stof(temp2);
				}
				else if (temp1 == "REPLACE_CANCEL") {
					replace_cancel = stof(temp2);
				}
				else if (temp1 == "SELECTION_CAP") {
					if (temp2.find("{CHOICE}") != -1) {
						temp2.insert(temp2.find("{CHOICE}"), choice);
						temp2.erase(temp2.find("{CHOICE}"), string("{CHOICE}").length());
					}
					if (temp2 == "{ACTIVE_MON_MOVE_SIZE}") {
						selection_cap = get_active_mon_move_size();
					}
					else if (temp2 == "{TEAM_SIZE}") {
						selection_cap = get_team_size();
					}
					else if (temp2.find("{MON_MOVE_COUNT:") == 0) {
						temp2.erase(temp2.find("{MON_MOVE_COUNT:"), string("{MON_MOVE_COUNT:").length());
						selection_cap = get_mon_move_size(stoi(temp2));
					}
					else if (temp2.find("{STORAGE_MON_MOVE_COUNT:") == 0) {
						temp2.erase(temp2.find("{STORAGE_MON_MOVE_COUNT:"), string("{STORAGE_MON_MOVE_COUNT:").length());
						selection_cap = get_storage_mon_move_size(stoi(temp2));
					}
					else
						selection_cap = stoi(temp2);
				}
				else if (temp1 == "HP_BARS") {
					std::getline(f, line);
					while (line != "END") {
						while (line.find("{CHOICE}") != -1) {
							line.insert(line.find("{CHOICE}"), choice);
							line.erase(line.find("{CHOICE}"), string("{CHOICE}").length());
						}
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						t.xmin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						t.ymin = stof(temp1);
						temp1 = line;
						t.s = to_string(stoi(line));
						hp_bars.push_back(t);
						std::getline(f, line);
					}
				}
				else if (temp1 == "BOXES") {
					std::getline(f, line);
					while (line != "END") {
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						b.xmin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						b.ymin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						b.length = stof(temp1);
						temp1 = line;
						b.height = stof(temp1);
						boxes.push_back(b);
						std::getline(f, line);
					}
				}
				else if (temp1 == "IMAGES") {
					std::getline(f, line);
					while (line != "END") {
						while (line.find("{CHOICE}") != -1) {
							line.insert(line.find("{CHOICE}"), choice);
							line.erase(line.find("{CHOICE}"), string("{CHOICE}").length());
						}
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						im.xmin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						im.ymin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						im.length = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						im.height = stof(temp1);
						im.filename = get_special_string(line);
						images.push_back(im);
						std::getline(f, line);
					}
				}
				else if (temp1 == "ARROWBOXES") {
					std::getline(f, line);
					while (line != "END") {
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						b.xmin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						b.ymin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						b.length = stof(temp1);
						temp1 = line;
						b.height = stof(temp1);
						arrowboxes.push_back(b);
						std::getline(f, line);
					}
				}
				else if (temp1.find("DEX") == 0) {
					int count = 0;
					std::getline(f, line);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.5f;
					std::map<string, bool> seen_map = get_seen_table();
					std::map<string, bool> caught = get_caught_table();
					std::map<string, bool>::iterator it;
					std::vector<string> seen;
					for (unsigned i = 1; i <= 151; ++i) {
						for (unsigned j = 1; j <= 151; ++j) {
							string check;
							check = to_string(i) + string("-") + to_string(j);
							if (seen_map[check])
								seen.push_back(check);
						}
					}
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = seen.size() - 1;
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (unsigned i = 0; i < seen.size(); ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:13:MON_TEMPLATE_NAME:") + seen[i]);
						if (caught[seen[i]])
							reserve_followup.push_back(string("DEX_MENU_CAUGHT:") + seen[i]); // TODO: DATA, CRY, AREA, QUIT
						else
							reserve_followup.push_back(string("DEX_MENU_SEEN:") + seen[i]); // TODO: DATA, CRY, AREA, QUIT
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					followup.push_back("");
					raw.push_back(t);
					for (unsigned i = 0; i < seen.size(); ++i) {
						if (caught[seen[i]])
							reserve.push_back(seen[i] + string("{BALL}"));
						else
							reserve.push_back(seen[i]);
					}
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.4f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("INVENTORY_") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1.erase(0, temp1.find("_") + 1);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_inventory_count(temp1);
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("ITEM:") + temp1 + string(":") + to_string(i));
						string effect = get_item_effect(get_special_string(string("ITEM:") + temp1 + string(":") + to_string(i)));
						if (is_menu(effect)) {
							reserve_followup.push_back(get_menu(effect));
						}
						else {
							reserve_followup.push_back("");
						}
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:") + to_string(int((b.length - 0.3) * 10.0)) + string(":ITEM_COUNT:") + temp1 + string(":") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("WITHDRAW_ITEM") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_inventory_storage_count(temp1);
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("ITEM_STORAGE:") + temp1 + string(":") + to_string(i));
						reserve_followup.push_back(string("EXCHANGE_STORAGE:0_") + to_string(get_item_storage_count(get_special_string(string("ITEM_STORAGE:") + temp1 + string(":") + to_string(i)))));
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:") + to_string(int((b.length - 0.3) * 10.0)) + string(":ITEM_STORAGE_COUNT:") + temp1 + string(":") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("RELEASE_PKMN") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_storage_use();
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("STORAGE_MON_NAME:") + to_string(i));
						reserve_followup.push_back(string("RELEASE_STATS_CANCEL:") + to_string(i));
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:13:STORAGE_MON_LEVEL:") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("WITHDRAW_PKMN") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_storage_use();
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("STORAGE_MON_NAME:") + to_string(i));
						reserve_followup.push_back(string("WITHDRAW_STATS_CANCEL:") + to_string(i));
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:13:STORAGE_MON_LEVEL:") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("DEPOSIT_PKMN") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_team_size();
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("TEAM_MON_NAME:") + to_string(i));
						reserve_followup.push_back(string("DEPOSIT_STATS_CANCEL:") + to_string(i));
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:13:TEAM_MON_LEVEL:") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("DEPOSIT_ITEM") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_inventory_count(temp1);
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("ITEM:") + temp1 + string(":") + to_string(i));
						reserve_followup.push_back(string("EXCHANGE_STORAGE:0_") + to_string(get_item_count(get_special_string(string("ITEM:") + temp1 + string(":") + to_string(i)))));
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:") + to_string(int((b.length - 0.3) * 10.0)) + string(":ITEM_COUNT:") + temp1 + string(":") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("SELL") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = get_inventory_count(temp1);
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("ITEM:") + temp1 + string(":") + to_string(i));
						if (get_item_cost(get_special_string(string("ITEM:") + temp1 + string(":") + to_string(i))) > 0)
							reserve_followup.push_back("EXCHANGE_SELL:" + to_string(get_item_cost(get_special_string(string("ITEM:") + temp1 + string(":") + to_string(i))) / 2) + string("_") + to_string(get_item_count(get_special_string(string("ITEM:") + temp1 + string(":") + to_string(i)))));
						else
							reserve_followup.push_back("ALERT:I can't buy that.");
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:") + to_string(int((b.length - 0.3) * 10.0)) + string(":ITEM_COUNT:") + temp1 + string(":") + to_string(i));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("BUY") == 0) {
					int count = 0;
					std::getline(f, line);
					temp1 = "ALL";
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.4f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					vector<string> items;
					while (choice.find(",") != -1) {
						string s = choice;
						s.erase(s.find(","), s.length());
						choice.erase(0, choice.find(",") + 1);
						items.push_back(s);
					}
					items.push_back(choice);
					max = items.size();
					string money_str = get_special_string(string("{MONEY}"));
					while (money_str.find("$") != -1)
						money_str.erase(money_str.find("$"), money_str.find("$") + 1);
					int money = stoi(money_str);
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(items[i]);
						if (money / get_item_cost(items[i]) > 0)
							reserve_followup.push_back("EXCHANGE_BUY:" + to_string(get_item_cost(items[i])) + string("_") + to_string(money / get_item_cost(items[i])));
						else
							reserve_followup.push_back("ALERT:I'm sorry, but you don't have enough money for that.");
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					reserve.push_back(string("CANCEL"));
					reserve_followup.push_back(string(""));
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("RIGHT_JUSTIFY:") + to_string(int((b.length - 0.3) * 10.0)) + string(":$") + to_string(get_item_cost(items[i])));
					}
					reserve.push_back(string(""));
					cancel_option = max;
					selection_cap = count + 1;

					x = b.xmin + 0.2;
					y = b.ymin + b.height - 0.5f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					update_reserves();
				}
				else if (temp1.find("EXCHANGE") == 0) {
					int count = 0;
					string menu_name = temp1;
					temp1 = choice;
					temp1.erase(temp1.find("_"), temp1.length());
					int cost = stoi(temp1);
					temp1 = choice;
					temp1.erase(0, temp1.find("_") + 1);
					int limit = 0;
					if (temp1 != "MON_STORAGE_MAX") {
						limit = stoi(temp1);
					}
					else {
						limit = STORAGE_MAX;
					}
					std::getline(f, line);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.xmin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.ymin = stof(temp2);
					temp2 = line;
					temp2.erase(temp2.find(" "), temp2.length());
					line.erase(0, line.find(" ") + 1);
					b.length = stof(temp2);
					temp2 = line;
					b.height = stof(temp2);
					boxes.push_back(b);
					std::getline(f, line);
					float x = b.xmin + 0.2;
					float y = b.ymin + b.height - 0.3f;
					selection_cap = ((b.height + 0.0001) - 0.3) / 0.2;
					max = limit;
					for (count = 0; (count < max) && (count + 1 < selection_cap); count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
						followup.push_back("");
					}
					for (int i = 0; i < max; ++i) {
						reserve.push_back(string("{TIMES}") + to_string(max - i));
						if (menu_name.find("SELL") != -1)
							reserve_followup.push_back(string("ALERT_YES_NO:I can pay $") + to_string((max - i)*cost) + string(" for that."));
						else if (menu_name.find("BUY") != -1)
							reserve_followup.push_back(string("ALERT_YES_NO:That will be $") + to_string((max - i)*cost) + string(", OK?"));
						else if (menu_name.find("STORAGE") != -1)
							reserve_followup.push_back(string(""));
						else
							reserve_followup.push_back(string(""));
					}
					t.xmin = x;
					t.ymin = y;
					t.height = 0.1;
					t.length = b.length - 0.2;
					t.s = string("R") + to_string(count);
					followup.push_back("");
					raw.push_back(t);
					for (int i = 0; i < max; ++i) {
						if (menu_name.find("STORAGE") != -1)
							reserve.push_back(string(""));
						else
							reserve.push_back(string("RIGHT_JUSTIFY:7:$") + to_string((max - i)*cost));
					}
					max--;
					selection_cap = count + 1;

					x = b.xmin + 0.7;
					y = b.ymin + b.height - 0.3f;
					for (count = 0; count + 1 <= selection_cap; count++) {
						t.xmin = x;
						t.ymin = y;
						t.height = 0.1;
						t.length = b.length - 0.2f;
						t.s = string("R") + to_string(count);
						y -= 0.2f;
						raw.push_back(t);
					}
					scroll = max;
					update_reserves();
				}
				else if (temp1 == "TEXT") {
					std::getline(f, line);
					while (line != "END") {
						while (line.find("{NEWLINE}") != -1) {
							int i;
							i = line.find("{NEWLINE}");
							line.erase(i, string("{NEWLINE}").length());
							line.insert(i, string("\n"));
						}
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						t.xmin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						t.ymin = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						t.length = stof(temp1);
						temp1 = line;
						temp1.erase(temp1.find(" "), temp1.length());
						line.erase(0, line.find(" ") + 1);
						t.height = stof(temp1);
						t.s = line;
						if (t.s.find("{CHOICE}") != -1) {
							t.s.insert(t.s.find("{CHOICE}"), choice);
							t.s.erase(t.s.find("{CHOICE}"), string("{CHOICE}").length());
						}
						t.s = get_special_string(t.s);
						if (t.s.find("{SINGLE}") == 0) {
							t.s.erase(0, string("{SINGLE}").length());
							t.s = get_special_string(t.s);
							t.s = string("{SINGLE}") + t.s;
						}
						while (t.s.find(string("{PLAYER_NAME}")) != -1) {
							int i;
							i = t.s.find(string("{PLAYER_NAME}"));
							t.s.erase(i, string("{PLAYER_NAME}").length());
							t.s.insert(i, get_special_string(string("{PLAYER_NAME}")));
						}
						while (t.s.find(string("{RIVAL_NAME}")) != -1) {
							int i;
							i = t.s.find(string("{RIVAL_NAME}"));
							t.s.erase(i, string("{RIVAL_NAME}").length());
							t.s.insert(i, get_special_string(string("{RIVAL_NAME}")));
						}
						while (t.s.find(string("{SEEN}")) != -1) {
							int i;
							i = t.s.find(string("{SEEN}"));
							t.s.erase(i, string("{SEEN}").length());
							t.s.insert(i, get_special_string(string("{SEEN}")));
						}
						while (t.s.find(string("{CAUGHT}")) != -1) {
							int i;
							i = t.s.find(string("{CAUGHT}"));
							t.s.erase(i, string("{CAUGHT}").length());
							t.s.insert(i, get_special_string(string("{CAUGHT}")));
						}
						raw.push_back(t);
						std::getline(f, line);
					}
				}
				else if (temp1 == "FOLLOW_UP") {
					std::getline(f, line);
					while (line != "END") {
						while (line.find("{NEWLINE}") != -1) {
							int i;
							i = line.find("{NEWLINE}");
							line.erase(i, string("{NEWLINE}").length());
							line.insert(i, string("\n"));
						}
						if (line.find("{CHOICE}") != -1) {
							line.insert(line.find("{CHOICE}"), choice);
							line.erase(line.find("{CHOICE}"), string("{CHOICE}").length());
						}
						if (line.find(":") != -1) {
							string s1 = line;
							string s2 = line;
							s1.erase(line.find(":"), s1.length());
							s2.erase(0, line.find(":") + 1);
							s2 = get_special_string(s2);
							line = s1 + string(":") + s2;
						}
						line = get_special_string(line);
						followup.push_back(line);
						std::getline(f, line);
					}
					break;
				}
			}
		}
		f.close();
	}
	unsigned i = 0;
	if (text_override != "") {
		i = 0;
		while (text_override.size() > 0) {
			raw[i].s = text_override;
			if (text_override.find("|") != -1) {
				raw[i].s.erase(raw[i].s.find("|"), raw[i].s.size());
				text_override.erase(0, text_override.find("|") + 1);
			}
			else {
				text_override = "";
			}
			i = i + 1;
		}
	}
	if (followup_override != "") {
		i = 0;
		while (followup_override.size() > 0) {
			followup[i] = followup_override;
			if (followup_override.find("|") != -1) {
				followup[i].erase(followup[i].find("|"), followup[i].size());
				followup_override.erase(0, followup_override.find("|") + 1);
			}
			else {
				followup_override = "";
			}
			i = i + 1;
		}
	}
	process_strings();
	initialized = true;
}

void menu::update_reserves() {
	int shift = 0;
	for (int i = 0; i < int(raw.size()); ++i) {
		if (i >= selection_cap && i % selection_cap == 0)
			shift += (1 + max - selection_cap);
		raw[i].s = reserve[i + scroll + shift];
		if (i < selection_cap)
			followup[i] = reserve_followup[i + scroll + shift];
	}
}

void menu::push_menu() {
	m.lock();
	index = g.draw_list.size();
	for (unsigned i = 0; i < boxes.size(); ++i) {
		g.push_box(boxes[i].xmin, boxes[i].ymin, boxes[i].length, boxes[i].height);
	}
	for (unsigned i = 0; i < arrowboxes.size(); ++i) {
		if (arrowboxes[i].right)
			g.push_arrow_box_right(arrowboxes[i].xmin, arrowboxes[i].ymin, arrowboxes[i].length, arrowboxes[i].height);
		else
			g.push_arrow_box_left(arrowboxes[i].xmin, arrowboxes[i].ymin, arrowboxes[i].length, arrowboxes[i].height);
	}
	for (unsigned i = 0; i < hp_bars.size(); ++i) {
		push_hp_bar_if_exists(hp_bars[i].xmin, hp_bars[i].ymin, stoi(hp_bars[i].s));
	}
	for (unsigned i = 0; i < images.size(); ++i) {
		g.push_quad_load(images[i].xmin, images[i].ymin, images[i].length, images[i].height, string("../resources/images/") + images[i].filename);
	}
	for (unsigned i = 0; i < display.size(); ++i) {
		int temp;
		temp = g.push_text(display[i].xmin, display[i].ymin, display[i].length, display[i].height, display[i].size, display[i].s);
		if (i == 0)
			step = temp;
	}
	m.unlock();
}

void menu::pop_menu() {
	m.lock();
	cursor = -1;
	g.draw_list.erase(g.draw_list.begin() + index, g.draw_list.end());
	m.unlock();
}

void menu::process_strings() {
	display.clear();
	for (unsigned i = 0; i < raw.size(); ++i) {
		display.push_back(raw[i]);
		if (display[i].s.find("SELECTION") != -1) {
			display[i].s.erase(display[i].s.find("SELECTION"), string("SELECTION").length());
			display[i].s = display[i].s + to_string(selection);
		}
		display[i].s = get_special_string(display[i].s);
	}
}

void menu::input(bool up, bool down, bool left, bool right, bool select, bool start, bool confirm, bool cancel) {
	if (!initialized)
		return;
	if (done)
		return;
	mutex2.lock();
	if (start) {
		start = false;
		confirm = true;
	}
	if (etype == ALERT) {
		if (start | select | confirm | cancel) {
			if (step >= int(raw[0].s.size())) {
				done = true;
			}
			else {
				while (step < int(raw[0].s.size()) && raw[0].s[step] == ' ')
					step++;
				raw[0].s.erase(0, step);
				process_strings();
				pop_menu();
				push_menu();
			}
			mutex2.unlock();
			return;
		}
	}
	else if (etype == SELECT) {
		if (up) {
			if (selection < columns) {
				selection = selection_cap - (columns - selection);
			}
			else {
				selection -= columns;
			}
		}
		if (down) {
			if (selection >= selection_cap - columns) {
				selection = selection + columns - selection_cap;
			}
			else {
				selection += columns;
			}
		}
		if (right) {
			if (columns > 1) {
				if (selection % columns < columns - 1) {
					selection++;
				}
				else {
					selection--;
				}
			}
		}
		if (left) {
			if (columns > 1) {
				if (selection % columns == 0) {
					selection += columns - 1;
				}
				else {
					selection--;
				}
			}
		}
		if (confirm | start) {
			done = true;
			mutex2.unlock();
			return;
		}
		if (cancel) {
			selection = replace_cancel;
			done = true;
			mutex2.unlock();
			return;
		}
		process_strings();
		pop_menu();
		push_menu();
	}
	else if (etype == SCROLL) {
		if (up) {
			if (selection - 1 < 0 && selection + scroll - 1 >= 0) {
				scroll--;
			}
			else if (selection - 1 >= 0) {
				selection--;
			}
		}
		if (down) {
			if (selection + 1 >= selection_cap && selection + scroll + 1 <= max) {
				scroll++;
			}
			else if (selection + 1 < selection_cap) {
				selection++;
			}
		}
		if (confirm | start) {
			done = true;
			mutex2.unlock();
			return;
		}
		if (cancel) {
			selection = -1;
			done = true;
			mutex2.unlock();
			return;
		}
		update_reserves();
		process_strings();
		pop_menu();
		push_menu();
	}
	mutex2.unlock();
}

vector<int> menu::main() {
	vector<int> choice;
	while (!done || (done && (selection >= 0) && (selection < (int)followup.size()) && (followup[selection] != ""))) {
		mutex2.lock();
		if ((etype == SELECT || etype == SCROLL) && cursor == -1) {
			cursor = g.draw_list.size();
			g.push_quad(display[0].xmin - 0.1f + cursor_offset_x, display[0].ymin - 0.1f + cursor_offset_y, 0.1f, 0.1f, g.tex[string("cursor-2.bmp")]);
		}
		if ((cursor > 0) && (selection != -1)) {
			g.draw_list[cursor].x = display[selection].xmin - 0.1f + cursor_offset_x;
			g.draw_list[cursor].y = display[selection].ymin + 0.1f + cursor_offset_y;
		}
		mutex2.unlock();
		if (etype == AUTO_FOLLOWUP) {
			done = true;
			selection = 0;
		}
		if (done && (selection >= 0) && (followup.size() > 0)) {
			vector<int> out;
			if (followup[selection] == "")
				break;
			out = do_menu(followup[selection]);
			if ((etype != AUTO_FOLLOWUP) && ((out.size() == 0) || (out[out.size() - 1] == -1))) {
				done = false;
			}
			else {
				choice = out;
				break;
			}
		}
		int a = 0;
	}
	pop_menu();
	if (etype != AUTO_FOLLOWUP) {
		choice.insert(choice.begin(), selection != -1 ? selection + scroll : selection);
		if ((choice.size() > 0) && (choice[0] == offset + cancel_option))
			choice[0] = -1;
		if (always_cancel)
			choice[0] = -1;
	}
	return choice;
}