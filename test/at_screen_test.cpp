
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
    if (my_as.find_img_from_screen(postion, "1.png"))
    {
        for (auto&& i : postion)
        {
            my_ai.move_to(i.first, i.second);
            //my_ai.click(at::auto_input::mouse_button_type::double_left);
            my_ai.wait(200);
        }
    }
}