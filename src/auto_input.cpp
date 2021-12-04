#include "auto_input.h"

namespace at {
	auto_input::two_tuple auto_input::screen_size()
	{
		return two_tuple(mw::get_system_metrics(SM_CXSCREEN), mw::get_system_metrics(SM_CYSCREEN));
	}

	auto_input::two_tuple auto_input::mouse_pos()
	{
		POINT point = {0};
		GetCursorPos(&point);
		return two_tuple(point.x, point.y);
	}

	bool auto_input::on_screen(int x, int y)
	{
		int screen_x = mw::get_system_metrics(SM_CXSCREEN);
		int screen_y = mw::get_system_metrics(SM_CYSCREEN);
		return (x>=0) && (y>=0) && (x<=screen_x) && (y<=screen_y);
	}

	bool auto_input::move_to(int x, int y, int millisecond_total, int change_per_millisecond, move_type moves)
	{
		auto temp_pos = mouse_pos();
		auto target_point = mw::user::trans_screen_to_absolute(x, y);
		auto source_point = mw::user::trans_screen_to_absolute(temp_pos.first, temp_pos.second);

		return _linear_move(std::move(source_point), std::move(target_point), 
			MOUSEEVENTF_MOVE| MOUSEEVENTF_ABSOLUTE| MOUSEEVENTF_VIRTUALDESK, 0,
			millisecond_total, change_per_millisecond, moves);
	}

	bool auto_input::move(int x, int y, int millisecond_total, int change_per_millisecond, move_type moves)
	{
		auto temp_pos = mouse_pos();
		auto target_point = mw::user::trans_screen_to_absolute(temp_pos.first + x, temp_pos.second + y);
		auto source_point = mw::user::trans_screen_to_absolute(temp_pos.first, temp_pos.second);

		return _linear_move(std::move(source_point), std::move(target_point),
			MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK, 0,
			millisecond_total, change_per_millisecond, moves);
	}

	bool auto_input::click(mouse_button_type buttons, int interval_millisecond, click_type clicks)
	{
		input_list temp_input_list;
		INPUT temp_input = { 0 };
		INPUT wait_input = { 0 };
		wait_input.type = wait_sign;
		wait_input.mi.dx = interval_millisecond;
		int cycle_time = 1;
		int down_item = 0, up_item = 0;

		switch (buttons)
		{
		case at::auto_input::mouse_button_type::double_left:
			cycle_time = 2;
		case at::auto_input::mouse_button_type::left:
			down_item = MOUSEEVENTF_LEFTDOWN;
			up_item = MOUSEEVENTF_LEFTUP;
			break;
		case at::auto_input::mouse_button_type::double_right:
			cycle_time = 2;
		case at::auto_input::mouse_button_type::right:
			down_item = MOUSEEVENTF_RIGHTDOWN;
			up_item = MOUSEEVENTF_RIGHTUP;
			break;
		case at::auto_input::mouse_button_type::double_middle:
			cycle_time = 2;
		case at::auto_input::mouse_button_type::middle:
			down_item = MOUSEEVENTF_MIDDLEDOWN;
			up_item = MOUSEEVENTF_MIDDLEUP;
			break;
		}

		if (clicks != click_type::down_and_up) cycle_time = 1;

		for (size_t i = 0; i < cycle_time; i++)
		{
			switch (clicks)
			{
			case at::auto_input::click_type::down:
				mw::user::write_mouse_event(&temp_input, 0, 0, down_item);
				temp_input_list.push_back(temp_input);
				break;
			case at::auto_input::click_type::up:
				mw::user::write_mouse_event(&temp_input, 0, 0, up_item);
				temp_input_list.push_back(temp_input);
				break;
			case at::auto_input::click_type::down_and_up:
				mw::user::write_mouse_event(&temp_input, 0, 0, down_item);
				temp_input_list.push_back(temp_input);
				temp_input_list.push_back(wait_input);
				mw::user::write_mouse_event(&temp_input, 0, 0, up_item);
				temp_input_list.push_back(temp_input);
				break;
			}	
		}

		if (is_record)
			temp_list.insert(temp_list.end(), temp_input_list.begin(), temp_input_list.end());
		execute_input_list(temp_input_list);
		return true;
	}

	bool auto_input::execute_input_list(input_list& il)
	{
		bool is_succeed = true;
		for (auto&& i : il) {
			if (i.type == wait_sign)
				mw::sleep(i.mi.dx);
			else is_succeed = is_succeed && mw::user::send_input(&i);
		}
		return is_succeed;
	}

	bool auto_input::_linear_move(two_tuple&& source_point, two_tuple&& target_point, DWORD flags, int scroll,  
		int millisecond_total, int change_per_millisecond, move_type moves)
	{
		if (millisecond_total < 0 || change_per_millisecond <= 0) return false;
		int cycle_time = millisecond_total / change_per_millisecond;

		input_list temp_input_list;
		INPUT temp_input = { 0 };
		INPUT wait_input = { 0 };
		wait_input.type = wait_sign;
		wait_input.mi.dx = change_per_millisecond;

		int x_increment = (target_point.first - source_point.first) / cycle_time;
		int y_increment = (target_point.second - source_point.second) / cycle_time;
		int scroll_increment = scroll / cycle_time;

		for (size_t i = 0; i < cycle_time; i++)
		{
			source_point.first += x_increment;
			source_point.second += y_increment;
			mw::user::write_mouse_event(&temp_input, source_point.first,
				source_point.second, flags, scroll_increment);
			temp_input_list.push_back(temp_input);
			temp_input_list.push_back(wait_input);
		}

		mw::user::write_mouse_event(&temp_input, target_point.first,
			target_point.second, flags, 0);
		temp_input_list.push_back(temp_input);
		temp_input_list.push_back(wait_input);

		if (is_record)
			temp_list.insert(temp_list.end(), temp_input_list.begin(), temp_input_list.end());
		execute_input_list(temp_input_list);
		return true;
	}

	bool auto_input::scroll(int scroll_counts, int millisecond_total, int change_per_millisecond)
	{
		two_tuple a, b;
		return _linear_move(std::move(a), std::move(b), MOUSEEVENTF_WHEEL, 
			scroll_counts, millisecond_total, change_per_millisecond);
	}

	bool auto_input::hscroll(int hscroll_counts, int millisecond_total, int change_per_millisecond)
	{
		two_tuple a, b;
		return _linear_move(std::move(a), std::move(b), MOUSEEVENTF_HWHEEL,
			hscroll_counts, millisecond_total, change_per_millisecond);
	}

	bool auto_input::key_down(WORD buttons)
	{
		INPUT temp_input = { 0 };
		mw::user::write_keyboard_event(&temp_input, buttons, 0, 0);
		if (is_record)
			temp_list.push_back(temp_input);
		return mw::user::send_input(&temp_input);
	}

	bool auto_input::key_up(WORD buttons)
	{
		INPUT temp_input = { 0 };
		mw::user::write_keyboard_event(&temp_input, buttons, KEYEVENTF_KEYUP, 0);
		if (is_record)
			temp_list.push_back(temp_input);
		return mw::user::send_input(&temp_input);
	}

};//at


