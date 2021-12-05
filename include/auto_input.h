#pragma once
#include "stdafx.h"

namespace at {
	/// <summary>
	/// 自动化输入类，目前支持键盘和鼠标
	/// </summary>
	class auto_input
	{
	public:
		using two_tuple = std::pair<int, int>;
		using input_list = std::vector<INPUT>;

		/// 等待信号，它用于指定等待一段时间
		static constexpr int wait_sign = 10086;

		enum class move_type :int
		{
			linear,
			smooth
		};

		enum class mouse_button_type :int {
			/// 鼠标左键
			left,
			/// 鼠标右键
			right,
			/// 鼠标中键
			middle,
			/// 鼠标左键双击
			double_left,
			/// 鼠标右键双击
			double_right,
			/// 鼠标中键双击
			double_middle
		};

		enum class click_type :int {
			/// 按下
			down,
			/// 弹起
			up,
			/// 按下然后弹起
			down_and_up
		};

	public:
		auto_input() {}
		~auto_input() {}

	public:
		/// <summary>
		/// 获取屏幕大小
		/// </summary>
		/// <returns>返回一个二元组，first即屏幕宽度，second即屏幕高度</returns>
		two_tuple screen_size();

		/// <summary>
		/// 获取鼠标位置
		/// </summary>
		/// <returns>返回一个二元组，first即鼠标x轴，second即鼠标y轴(相对于屏幕)</returns>
		two_tuple mouse_pos();

		/// <summary>
		/// 判断指定坐标是否在屏幕上，还是屏幕外
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <returns>指定坐标是否在屏幕上</returns>
		bool on_screen(int x, int y);

		/// <summary>
		/// 将当前鼠标移动到指定位置
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="millisecond_total">移动总共需要的毫秒数</param>
		/// <param name="change_per_millisecond">多少毫秒变化一次(移动一次)</param>
		/// <param name="moves">鼠标滑动的类型，目前只支持linear</param>
		/// <returns>操作是否成功</returns>
		bool move_to(int x, int y, int millisecond_total = 100, int change_per_millisecond = 5,
			move_type moves = move_type::linear);

		/// <summary>
		/// 将当前鼠标移动x轴距离和y轴距离
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="millisecond_total">移动总共需要的毫秒数</param>
		/// <param name="change_per_millisecond">多少毫秒变化一次(移动一次)</param>
		/// <param name="moves">鼠标滑动的类型，目前只支持linear</param>
		/// <returns>操作是否成功</returns>
		bool move(int x, int y, int millisecond_total = 100, int change_per_millisecond = 5,
			move_type moves = move_type::linear);

		/// <summary>
		/// 将当前鼠标拖拉到指定位置，类似于框选和移动文件的感觉
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="buttons">鼠标按下的哪个键，默认为左键，不能为双击</param>
		/// <returns>操作是否成功</returns>
		bool drag_to(int x, int y, mouse_button_type buttons = mouse_button_type::left)
		{
			if (buttons == mouse_button_type::double_left ||
				buttons == mouse_button_type::double_right ||
				buttons == mouse_button_type::double_middle)
				return false;

			return click_down(buttons) && move_to(x, y) && click_up(buttons);
		}

		/// <summary>
		/// 将当前鼠标拖拉x轴距离和y轴距离
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="buttons">鼠标按下的哪个键，默认为左键，不能为双击</param>
		/// <returns>操作是否成功</returns>
		bool drag(int x, int y, mouse_button_type buttons = mouse_button_type::left)
		{
			if (buttons == mouse_button_type::double_left ||
				buttons == mouse_button_type::double_right ||
				buttons == mouse_button_type::double_middle)
				return false;

			return click_down(buttons) && move(x, y) && click_up(buttons);
		}

		/// <summary>
		/// 将当前鼠标移动到指定位置，然后按下指定鼠标按键
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="buttons">鼠标按下的哪个键，默认为左键</param>
		/// <param name="interval_millisecond">键按下和弹起的间隔，同时也是双击的间隔(若指定为双击)</param>
		/// <param name="clicks">按键的动作，可以是按下不弹起，或者弹起，或者按下，然后弹起</param>
		/// <returns>操作是否成功</returns>
		bool click(int x, int y, mouse_button_type buttons = mouse_button_type::left, int interval_millisecond = 20,
			click_type clicks = click_type::down_and_up)
		{
			return move_to(x, y) && click(buttons, interval_millisecond, clicks);
		}

		/// <summary>
		/// 鼠标在当前鼠标位置按下指定鼠标按键
		/// </summary>
		/// <param name="buttons">鼠标按下的哪个键，默认为左键</param>
		/// <param name="interval_millisecond">键按下和弹起的间隔，同时也是双击的间隔(若指定为双击)</param>
		/// <param name="clicks">按键的动作，可以是按下不弹起，或者弹起，或者按下，然后弹起</param>
		/// <returns>操作是否成功</returns>
		bool click(mouse_button_type buttons = mouse_button_type::left, int interval_millisecond = 20,
			click_type clicks = click_type::down_and_up);

		/// <summary>
		/// 将当前鼠标移动到指定位置，然后按下指定鼠标按键(按下不弹起，即按下不动)
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="buttons">鼠标按下的哪个键，默认为左键</param>
		/// <returns>操作是否成功</returns>
		bool click_down(int x, int y, mouse_button_type buttons = mouse_button_type::left)
		{
			return move_to(x, y) && click(buttons, 0, click_type::down);
		}

		/// <summary>
		/// 将当前鼠标移动到指定位置，然后弹起指定鼠标按键
		/// </summary>
		/// <param name="x">相对于屏幕的x轴坐标</param>
		/// <param name="y">相对于屏幕的y轴坐标</param>
		/// <param name="buttons">鼠标弹起的哪个键，默认为左键</param>
		/// <returns>操作是否成功</returns>
		bool click_up(int x, int y, mouse_button_type buttons = mouse_button_type::left)
		{
			return move_to(x, y) && click(buttons, 0, click_type::up);
		}

		/// <summary>
		/// 鼠标在当前鼠标位置按下指定鼠标按键(按下不弹起，即按下不动)
		/// </summary>
		/// <param name="buttons">鼠标按下的哪个键，默认为左键</param>
		/// <returns>操作是否成功</returns>
		bool click_down(mouse_button_type buttons = mouse_button_type::left)
		{
			return click(buttons, 0, click_type::down);
		}

		/// <summary>
		/// 鼠标在当前鼠标位置弹起指定鼠标按键
		/// </summary>
		/// <param name="buttons">鼠标弹起的哪个键，默认为左键</param>
		/// <returns>操作是否成功</returns>
		bool click_up(mouse_button_type buttons = mouse_button_type::left)
		{
			return click(buttons, 0, click_type::up);
		}

		/// <summary>
		/// 鼠标滚轮移动指定量
		/// </summary>
		/// <param name="scroll_counts">滚轮滚动的量</param>
		/// <param name="millisecond_total">滚动总共的毫秒数</param>
		/// <param name="change_per_millisecond">多少毫秒变化一次(移动一次)</param>
		/// <returns>操作是否成功</returns>
		bool scroll(int scroll_counts, int millisecond_total = 100, int change_per_millisecond = 5);

		/// <summary>
		/// 鼠标水平滚轮移动指定量(水平滚动)
		/// </summary>
		/// <param name="scroll_counts">滚轮滚动的量</param>
		/// <param name="millisecond_total">滚动总共的毫秒数</param>
		/// <param name="change_per_millisecond">多少毫秒变化一次(移动一次)</param>
		/// <returns>操作是否成功</returns>
		bool hscroll(int hscroll_counts, int millisecond_total = 100, int change_per_millisecond = 5);

	public:
		// 有关buttons的值，请看这里：https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

		/// <summary>
		/// 按下并弹起指定键盘按键
		/// </summary>
		/// <param name="buttons">指定按钮，它应该是VK_开头的宏，或是字符'0'到'9'或'A'到'Z'，后者指定对应的键盘数字和字母，注意不要是小写</param>
		/// <param name="interval">按下和弹起的间隔时间</param>
		/// <returns>操作是否成功</returns>
		bool press(WORD buttons, int interval = 20)
		{
			return key_down(buttons) && wait(interval) && key_up(buttons);
		}

		/// <summary>
		/// 按下并弹起一系列指定键盘按键，根据列表元素的先后顺序来按下和弹起指定按键
		/// </summary>
		/// <param name="buttons_list">按键列表，该函数会按其顺序按下和弹起指定键盘按键</param>
		/// <param name="interval">按下和弹起的间隔时间</param>
		/// <returns>操作是否成功</returns>
		bool press(std::initializer_list<WORD> buttons_list, int interval = 20)
		{
			bool is_ok = true;
			for (auto&& b : buttons_list)
				is_ok = is_ok && press(b, interval);
			return is_ok;
		}

		/// <summary>
		/// 按下并弹起一系列指定键盘按键，根据列表元素的先后顺序来按下和弹起指定按键
		/// </summary>
		/// <param name="buttons_list">按键列表，该函数会按其顺序按下和弹起指定键盘按键</param>
		/// <param name="interval">按下和弹起的间隔时间</param>
		/// <returns>操作是否成功</returns>
		bool press(std::initializer_list<const char*> buttons_name_list, int interval = 20)
		{
			bool is_ok = true;
			for (auto&& b : buttons_name_list)
				is_ok = is_ok && press(enum_mapping(b), interval);
			return is_ok;
		}


		/// <summary>
		/// 按下指定键盘按键(但不弹起)
		/// </summary>
		/// <param name="buttons">指定按钮，它应该是VK_开头的宏，或是字符'0'到'9'或'A'到'Z'，后者指定对应的键盘数字和字母，注意不要是小写</param>
		/// <returns>操作是否成功</returns>
		bool key_down(WORD buttons);

		/// <summary>
		/// 弹起指定键盘按键
		/// </summary>
		/// <param name="buttons">指定按钮，它应该是VK_开头的宏，或是字符'0'到'9'或'A'到'Z'，后者指定对应的键盘数字和字母，注意不要是小写</param>
		/// <returns>操作是否成功</returns>
		bool key_up(WORD buttons);


	public:
		/// <summary>
		/// 等待指定毫秒数
		/// </summary>
		/// <param name="millisecond">需要等待的毫秒数</param>
		/// <returns>操作是否成功</returns>
		bool wait(int millisecond = 1000) {
			if (is_record)
			{
				INPUT wait_input = { 0 };
				wait_input.type = wait_sign;
				wait_input.mi.dx = millisecond;
				temp_list.push_back(wait_input);
			}
			mw::sleep(millisecond);
			return true;
		}

		/// <summary>
		/// 开始记录输入，从调用该函数开始，到调用end_record结束，期间调用的所有输入都会被记录
		/// </summary>
		/// <returns>操作是否成功</returns>
		bool begin_record()
		{
			temp_list.clear();
			is_record = true;
			return true;
		}

		/// <summary>
		/// 结束记录输入，该函数会返回一个输入列表，其中包含从调用begin_record开始，到调用end_record结束期间调用的所有输入
		/// </summary>
		/// <returns>操作是否成功</returns>
		input_list end_record()
		{
			is_record = false;
			return temp_list;
		}

		/// <summary>
		/// 执行指定输入列表，该输入列表应该是调用end_record的返回值，它会回放开始记录到结束记录期间的所有输入操作
		/// </summary>
		/// <param name="il">指定输入列表</param>
		/// <returns>操作是否成功</returns>
		bool execute_input_list(input_list& il);

	private:
		bool _linear_move(two_tuple&& source_point, two_tuple&& target_point, DWORD flags, int scroll = 0,
			int millisecond_total = 100, int change_per_millisecond = 5, move_type moves = move_type::linear);


	private:
		bool is_record = false;

		input_list temp_list;

	public:
		struct enum_value_type {
			enum_value_type(int val) :val(val) {};
			int val;
			operator mouse_button_type() const {
				return mouse_button_type(val);
			}
			operator click_type() const {
				return click_type(val);
			}
			operator WORD() const {
				return WORD(val);
			}
		};

		/// <summary>
		/// 将对应的枚举类型的字符串映射为枚举类型的值
		/// </summary>
		/// <param name="enum_name">指定的枚举名字</param>
		/// <returns>若成功，返回指定枚举名字对应的枚举值，否则返回-1</returns>
		static enum_value_type enum_mapping(const std::string& enum_name)
		{
			static const std::unordered_map<std::string, int> _input_type_mapping_dict = {
				{"left", (int)mouse_button_type::left},
				{"right", (int)mouse_button_type::right},
				{"middle", (int)mouse_button_type::middle},
				{"double_left", (int)mouse_button_type::double_left},
				{"double_right", (int)mouse_button_type::double_right},
				{"double_middle", (int)mouse_button_type::double_middle},
				{"down", (int)click_type::down},
				{"up", (int)click_type::up},
				{"up_and_down", (int)click_type::down_and_up},

				// 键盘
				{"back", VK_BACK},
				{"tab", VK_TAB},
				{"enter", VK_RETURN},
				{"shift", VK_SHIFT},
				{"ctrl", VK_CONTROL},
				{"alt", VK_MENU},
				{"pause", VK_PAUSE},
				{"caps_lock", VK_CAPITAL},
				{"esc", VK_ESCAPE},
				{"space", VK_SPACE},
				{"page_up", VK_PRIOR},
				{"page_down", VK_NEXT},
				{"end", VK_END},
				{"home", VK_HOME},
				{"arrow_left", VK_LEFT},
				{"arrow_up", VK_UP},
				{"arrow_right", VK_RIGHT},
				{"arrow_down", VK_DOWN},
				{"print", VK_PRINT},
				{"insert", VK_INSERT},
				{"delete", VK_DELETE},
				{"A", 'A'},{"B", 'B'},{"C", 'C'},{"D", 'D'},{"E", 'E'},
				{"F", 'F'},{"G", 'G'},{"H", 'H'},{"I", 'I'},{"J", 'J'},
				{"K", 'K'},{"L", 'L'},{"M", 'M'},{"N", 'N'},{"O", 'O'},
				{"P", 'P'},{"Q", 'Q'},{"R", 'R'},{"S", 'S'},{"T", 'T'},
				{"U", 'U'},{"V", 'V'},{"W", 'W'},{"X", 'X'},{"Y", 'Y'},
				{"Z", 'Z'},
				{"a", 'A'},{"b", 'B'},{"c", 'C'},{"d", 'D'},{"e", 'E'},
				{"f", 'F'},{"g", 'G'},{"h", 'H'},{"i", 'I'},{"j", 'J'},
				{"k", 'K'},{"l", 'L'},{"m", 'M'},{"n", 'N'},{"o", 'O'},
				{"p", 'P'},{"q", 'Q'},{"r", 'R'},{"s", 'S'},{"t", 'T'},
				{"u", 'U'},{"v", 'V'},{"w", 'W'},{"x", 'X'},{"y", 'Y'},
				{"z", 'Z'},
				{"0", '0'},{"1", '1'},{"2", '2'},{"3", '3'},{"4", '4'},
				{"5", '5'},{"6", '6'},{"7", '7'},{"8", '8'},{"9", '9'},
				{"num0", VK_NUMPAD0},{"num1", VK_NUMPAD1},{"num2", VK_NUMPAD2},{"num3", VK_NUMPAD3},{"num4", VK_NUMPAD4},
				{"num5", VK_NUMPAD5},{"num6", VK_NUMPAD6},{"num7", VK_NUMPAD7},{"num8", VK_NUMPAD8},{"num9", VK_NUMPAD9},
				{"win_left", VK_LWIN},
				{"win_right", VK_RWIN},
				{"f1", VK_F1},{"f2", VK_F2},{"f3", VK_F3},{"f4", VK_F4},{"f5", VK_F5},
				{"f6", VK_F6},{"f7", VK_F7},{"f8", VK_F8},{"f9", VK_F9},{"f10", VK_F10},
				{"f11", VK_F11},{"f12", VK_F12},
				{"num_lock", VK_NUMLOCK},
				{"scroll_lock", VK_SCROLL},
				{"+", VK_OEM_PLUS},
				{",", VK_OEM_COMMA},
				{"-", VK_OEM_MINUS},
				{".", VK_OEM_PERIOD},
				{";", VK_OEM_1},
				{"/", VK_OEM_2},
				{"`", VK_OEM_3},
				{"[", VK_OEM_4},
				{"\\", VK_OEM_5},
				{"]", VK_OEM_6},
				{"'", VK_OEM_7}
			};

			auto iter = _input_type_mapping_dict.find(enum_name);

			if (iter == _input_type_mapping_dict.end())
				return -1;
			else return iter->second;
		};
	};
};//at

