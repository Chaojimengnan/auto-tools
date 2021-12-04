
class auto_input_test : public testing::Test
{
protected:
    void SetUp() override {

    }

    at::auto_input my_ai;
};

void print_two_tuple(const at::auto_input::two_tuple& the_two_tuple)
{
    std::cout << "first:" << the_two_tuple.first << ", second:" << the_two_tuple.second << "\n";
}

//TEST_F(auto_input_test, test_mouse_on_screen) {
//    auto screen = my_ai.screen_size();
//    auto mouse_pos = my_ai.mouse_pos();
//    //print_two_tuple(screen);
//    //print_two_tuple(mouse_pos);
//
//    EXPECT_TRUE(my_ai.on_screen(mouse_pos.first, mouse_pos.second));
//    EXPECT_FALSE(my_ai.on_screen(-1, mouse_pos.second));
//    EXPECT_FALSE(my_ai.on_screen(mouse_pos.first, -1));
//}
//
//TEST_F(auto_input_test, test_mouse_moves) {
//    my_ai.move_to(100, 100);
//    my_ai.move_to(200, 100);
//    my_ai.move_to(200, 200);
//    my_ai.move_to(200, 100);
//    my_ai.move_to(100, 100);
//    my_ai.move(1000, 1000);
//    my_ai.move_to(10, 10);
//    my_ai.drag_to(300, 300);
//    my_ai.drag_to(10, 10);
//}
//
//TEST_F(auto_input_test, test_mouse_scroll) {
//    my_ai.move(1000, 1000);
//    my_ai.click();
//    my_ai.scroll(1000);
//    my_ai.scroll(-1000);
//}
//
//TEST_F(auto_input_test, test_mouse_record) {
//
//    my_ai.begin_record();
//    my_ai.move_to(100, 100);
//    my_ai.move_to(200, 100);
//    my_ai.move_to(200, 200);
//    my_ai.move_to(200, 100);
//    my_ai.move_to(100, 100);
//    my_ai.wait(1000);
//    my_ai.move_to(1000, 1000);
//    my_ai.move_to(10, 10);
//    my_ai.drag_to(300, 300);
//    my_ai.drag_to(10, 10);
//    auto action_2 = my_ai.end_record();
//
//    my_ai.begin_record();
//    my_ai.move(1000, 1000);
//    my_ai.click();
//    my_ai.scroll(1000);
//    my_ai.scroll(-1000);
//    auto action_1 = my_ai.end_record();
//
//
//    my_ai.execute_input_list(action_2);
//    my_ai.execute_input_list(action_1);
//
//}
//
//TEST_F(auto_input_test, test_keybd_test) {
//
//    /*my_ai.key_down(VK_MENU);
//
//    my_ai.press(VK_TAB);
//    my_ai.wait();
//    my_ai.press(VK_TAB);
//    my_ai.wait();
//    my_ai.press(VK_TAB);
//    my_ai.wait();
//    my_ai.key_up(VK_MENU);*/
//    my_ai.wait(2000);
//    my_ai.press({ 'A', 'B', 'C', 'D', '1', '2', '3' }, 200);
//
//}