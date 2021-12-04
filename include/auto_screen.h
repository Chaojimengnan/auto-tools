#pragma once

// https://github.com/asweigart/pyautogui/blob/master/docs/simplified-chinese.ipynb
// https://blog.csdn.net/qq_18984151/article/details/79689732
// https://blog.csdn.net/qq_30815237/article/details/86812716
// https://docs.opencv.org/4.x/df/dfb/group__imgproc__object.html#ga586ebfb0a7fb604b35a23d85391329be
namespace at {
	class auto_screen
	{
	public:
		using two_tuple = std::pair<int, int>;

	public:
		/// <summary>
		/// 给屏幕截图，并返回其位图图形对象的句柄
		/// </summary>
		/// <returns>带有屏幕信息的位图图形对象的句柄</returns>
		HBITMAP screen_slot()
		{
			HDC screen_handle = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
			HDC	memory_handle = CreateCompatibleDC(screen_handle);
			int	screen_width = GetSystemMetrics(SM_CXSCREEN);
			int	screen_height = GetSystemMetrics(SM_CYSCREEN);
			auto bitmap_handle = CreateCompatibleBitmap(screen_handle, screen_width, screen_height);
			auto old_handle = (HBITMAP)SelectObject(memory_handle, bitmap_handle);

			// 将屏幕数据传到位图中
			BitBlt(memory_handle, 0, 0, screen_width, screen_height, screen_handle, 0, 0, SRCCOPY);
			SelectObject(memory_handle, old_handle);

			DeleteDC(screen_handle);
			DeleteDC(memory_handle);
			return bitmap_handle;
		}

		/// <summary>
		/// 将位图句柄中的屏幕数据转换为opencv能够接收的矩阵数据
		/// </summary>
		/// <param name="bitmap">带有屏幕信息的位图图形对象的句柄</param>
		/// <returns>带有屏幕信息的矩阵</returns>
		cv::Mat bitmap_to_cv_mat(HBITMAP bitmap)
		{
			BITMAP bmp {0};
			GetObject(bitmap, sizeof(BITMAP), &bmp);
			int channels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;

			cv::Mat temp_mat;
			temp_mat.create(bmp.bmHeight, bmp.bmWidth , CV_MAKETYPE(CV_8U, 4));
			GetBitmapBits(bitmap, bmp.bmHeight * bmp.bmWidth * channels, temp_mat.data);
			return temp_mat;
		}

		/// <summary>
		/// 从屏幕中定位指定文件名的图片的位置，并将其返回，当信心小于指定值时，返回false，否则返回true
		/// </summary>
		/// <param name="img_postion">[out]返回指定图片在屏幕中的位置，当函数返回true时，这个值才有意义</param>
		/// <param name="img_file_name">要定位的图片文件的文件名</param>
		/// <param name="confidence">至少需要的信心，它是一个0到1的值</param>
		/// <returns>当信心小于指定值时，返回false，否则返回true</returns>
		bool find_img_from_screen(std::vector<two_tuple>& img_postion, const std::string& img_file_name, double confidence = 0.9f, bool return_all = true)
		{
			cv::Mat screen_image_3channel, result;

			auto template_image = cv::imread(img_file_name);
			

			auto bitmap_handle = screen_slot();
			auto screen_image = bitmap_to_cv_mat(bitmap_handle);

			DeleteObject(bitmap_handle);

			screen_image_3channel = screen_image;
			cv::cvtColor(screen_image, screen_image_3channel, 1);

			int result_cols = screen_image_3channel.cols - template_image.cols + 1;
			int result_rows = screen_image_3channel.rows - template_image.rows + 1;
			result.create(result_cols, result_rows, CV_32FC1); 

			cv::matchTemplate(screen_image_3channel, template_image, result, cv::TemplateMatchModes::TM_SQDIFF_NORMED);
			//cv::normalize(result, result, 0, 1, cv::NormTypes::NORM_MINMAX);
			

			double predict_confidence = 0;
			cv::Point matched_point;
			bool already_has_same_point_near = false;

			static auto check_boundary = [&img_postion, &already_has_same_point_near, &template_image](int x, int y) {

				for (auto iter = img_postion.rbegin(); iter != img_postion.rend(); ++iter)
				{
					int left = iter->first - template_image.cols;
					int right = iter->first;
					int top = iter->second - template_image.rows;
					int bottom = iter->second;

					if (right >= x && left <= x && top <= y && bottom >= y)
					{
						already_has_same_point_near = true; break;
					}
				}
			};


			if (!return_all)
			{
				cv::minMaxLoc(result, &predict_confidence, nullptr, &matched_point);
				img_postion.push_back({ matched_point.x + template_image.cols / 2 , matched_point.y + template_image.rows / 2 });
			}
			else {
				for (size_t x = 0; x < result.cols; ++x)
					for (size_t y = 0; y < result.rows; ++y)
						if (1 - result.at<float>(y, x) >= confidence)
						{
							check_boundary(x, y);
							if (!already_has_same_point_near)
								img_postion.push_back({ x + template_image.cols / 2, y + template_image.rows / 2 });
							already_has_same_point_near = false;
							y += static_cast<size_t>(template_image.rows) - 1;
						}
			}
			
			return img_postion.size();
		}

	};
};//at


