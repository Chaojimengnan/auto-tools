
class auto_screen_test : public testing::Test
{
protected:
    void SetUp() override {

    }

    at::auto_input my_ai;
    at::auto_screen my_as;
};

TEST_F(auto_screen_test, test_screen_catch) {
    std::vector<at::auto_input::two_tuple> postion;
    //if (my_as.find_img_from_screen(postion, "1.png"))
    //{
    //    for (auto&& i : postion)
    //    {
    //        my_ai.move_to(i.first, i.second);
    //        //my_ai.click(at::auto_input::mouse_button_type::double_left);
    //        my_ai.click(at::auto_input::enum_mapping("double_right"));
    //        my_ai.wait(1000);
    //    }
    //}

    my_ai.wait(2000);
    /*my_ai.press({
        at::auto_input::enum_mapping("caps_lock"),
        at::auto_input::enum_mapping("I"),
        at::auto_input::enum_mapping("caps_lock"),
        at::auto_input::enum_mapping("space"),
        at::auto_input::enum_mapping("l"),
        at::auto_input::enum_mapping("o"),
        at::auto_input::enum_mapping("v"),
        at::auto_input::enum_mapping("e"),
        at::auto_input::enum_mapping("space"),
        at::auto_input::enum_mapping("y"),
        at::auto_input::enum_mapping("o"),
        at::auto_input::enum_mapping("u")
        });*/

    my_ai.press({"q","u","e","s","h","i","1","enter"});

}