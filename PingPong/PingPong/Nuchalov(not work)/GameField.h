#include "Settings.h"
#pragma once

namespace PingPong {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ GameField
	/// </summary>
	public ref class GameField : public System::Windows::Forms::Form
	{
	private:
		Button^ settingsButton;
		Settings^ settingsForm;
		bool gamePaused;
		Label^ scoreLabel;
		Timer^ timer;
		float ball_x, ball_y;
		int velocity_x, velocity_y;
		float ball_speed;
		int ball_size = 10;
		float left_paddle_y, right_paddle_y;
		int paddle_width = 15, paddle_height = 80;
		int field_width, field_height;
		int left_paddle_x = 20, right_paddle_x;
		Color leftPaddleColor;
		Color rightPaddleColor;
		Color ballColor;

		int score_left = 0, score_right = 0;
		bool keyW = false, keyS = false, keyUp = false, keyDown = false;

		Random^ rnd;
	public:
		GameField(void)
		{
			InitializeComponent();
			field_height = this->ClientSize.Height;
			field_width = this->ClientSize.Width;
			right_paddle_x = field_width - 20 - paddle_width;
			initGame();
		}


	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~GameField()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// GameField
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 561);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"GameField";
			this->Text = L"GameField";

			settingsButton = gcnew Button();
			settingsButton->Text = "Settings";
			settingsButton->Location = Drawing::Point(10, 10);
			settingsButton->Size = Drawing::Size(80, 30);
			settingsButton->Click += gcnew EventHandler(this, &GameField::OpenSettings);
			this->Controls->Add(settingsButton);

			settingsForm = nullptr;
			gamePaused = false;


			scoreLabel = gcnew Label();
			scoreLabel->AutoSize = true;
			scoreLabel->Font = gcnew Drawing::Font(L"Arial", 24, FontStyle::Bold);
			scoreLabel->ForeColor = Color::Black;
			scoreLabel->BackColor = Color::Transparent;
			scoreLabel->Location = Drawing::Point(350, 20);
			scoreLabel->Text = L"0 : 0";
			this->Controls->Add(scoreLabel);

			this->ResumeLayout(false);

			


			rnd = gcnew Random();

			timer = gcnew Timer();
			timer->Interval = 16;
			timer->Tick += gcnew EventHandler(this, &GameField::gameLoop);
			timer->Start();

			this->KeyDown += gcnew KeyEventHandler(this, &GameField::OnKeyDown);
			this->KeyUp += gcnew KeyEventHandler(this, &GameField::OnKeyUp);
			this->Paint += gcnew PaintEventHandler(this, &GameField::OnPaint);
		}

		void initGame() {
			left_paddle_y = (field_height - paddle_height) / 2.f;
			right_paddle_y = left_paddle_y;
			score_left = score_right = 0;
			resetBall();
		}

		void resetBall() {
			ball_y = (field_height - ball_size) / 2.f;
			ball_x = (field_width - ball_size) / 2.f;
			velocity_x = (rnd->Next(2) == 0) ? 1 : -1;
			velocity_y = (rnd->Next(3) - 1);
			if (velocity_y == 0) velocity_y = 1;
			ball_speed = 3.f;
		}

		void OnKeyDown(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::W)			keyW = true;
			if (e->KeyCode == Keys::S)			keyS = true;
			if (e->KeyCode == Keys::Up)			keyUp = true;
			if (e->KeyCode == Keys::Down)		keyDown = true;
			e->SuppressKeyPress = true;
		}

		void OnKeyUp(Object^ sender, KeyEventArgs^ e) {
			if (e->KeyCode == Keys::W)			keyW = false;
			if (e->KeyCode == Keys::S)			keyS = false;
			if (e->KeyCode == Keys::Up)			keyUp = false;
			if (e->KeyCode == Keys::Down)		keyDown = false;
		}

		void OnPaint(Object^ sender, PaintEventArgs^ e) {
			Graphics^ graphics = e->Graphics;
			graphics->Clear(Color::White);
			
			SolidBrush^ leftBrush = gcnew SolidBrush(leftPaddleColor);
			SolidBrush^ rightBrush = gcnew SolidBrush(rightPaddleColor);
			SolidBrush^ ballBrush = gcnew SolidBrush(ballColor);
			
			graphics->FillRectangle(leftBrush, left_paddle_x, static_cast<int>(left_paddle_y), paddle_width, paddle_height);
			graphics->FillRectangle(rightBrush, right_paddle_x, static_cast<int>(right_paddle_y), paddle_width, paddle_height);
			graphics->FillEllipse(ballBrush, static_cast<int>(ball_x), static_cast<int>(ball_y), ball_size, ball_size);
			
			delete leftBrush;
			delete rightBrush;
			delete ballBrush;
			graphics->FillRectangle(Brushes::Black, left_paddle_x
				, static_cast<int>(left_paddle_y)
				, paddle_width
				, paddle_height);
			graphics->FillRectangle(Brushes::Black, right_paddle_x
				, static_cast<int>(right_paddle_y)
				, paddle_width
				, paddle_height);
			graphics->FillEllipse(Brushes::Red
				, static_cast<int>(ball_x)
				, static_cast<int>(ball_y)
				, ball_size
				, ball_size);
		}

		void gameLoop(Object^ sender, EventArgs^ e) {

			if (gamePaused || settingsForm != nullptr && settingsForm->Visible) {
				return;
			}

			int paddle_move = 8;
			if (keyW)	left_paddle_y -= paddle_move;
			if (keyS)	left_paddle_y += paddle_move;
			if (keyUp)	right_paddle_y -= paddle_move;
			if (keyDown)right_paddle_y += paddle_move;

			if (left_paddle_y < 0) left_paddle_y = 0;
			if (left_paddle_y > field_height - paddle_height)
				left_paddle_y = field_height - paddle_height;
			if (right_paddle_y < 0) right_paddle_y = 0;
			if (right_paddle_y > field_height - paddle_height)
				right_paddle_y = field_height - paddle_height;

			ball_speed += 0.005f;
			if (ball_speed > 8.f) ball_speed = 8.f;
			ball_x += velocity_x * ball_speed;
			ball_y += velocity_y * ball_speed;

			if (ball_y <= 0 || ball_y >= field_height - ball_size)
				velocity_y = -velocity_y;

			if (ball_x <= left_paddle_x + paddle_width &&
				ball_x >= left_paddle_x &&
				ball_y + ball_size >= left_paddle_y &&
				ball_y <= left_paddle_y + paddle_height
				)
				if (velocity_x < 0) {
					velocity_x = -velocity_x;
					ball_x = left_paddle_x + paddle_width;
					adjustBallAngle(left_paddle_y);
				}

			if (ball_x + ball_size >= right_paddle_x &&
				ball_x <= right_paddle_x + paddle_width &&
				ball_y + ball_size >= right_paddle_y &&
				ball_y <= right_paddle_y + paddle_height
				)
				if (velocity_x > 0) {
					velocity_x = -velocity_x;
					ball_x = right_paddle_x - ball_size;
					adjustBallAngle(right_paddle_y);
				}

			if (ball_x < 0) {
				++score_right;
				resetBall();
			}
			if (ball_x + ball_size > field_width) {
				++score_left;
				resetBall();
			}

			scoreLabel->Text = score_left + L" : " + score_right;

			this->Invalidate();
		}

		void adjustBallAngle(float paddle_pos) {
			float hit_pos = ((ball_y + ball_size) / 2.f - paddle_pos) / paddle_height;
			velocity_y = Math::Round(hit_pos - 0.5f) * 2.f;
			if (velocity_y == 0) velocity_y = 1;
		}

		void OpenSettings(Object^ sender, EventArgs^ e) {
			if (settingsForm == nullptr) settingsForm = gcnew Settings();

			gamePaused = true;
			timer->Enabled = false;

			settingsForm->Owner = this;
			settingsForm->ShowDialog();
			this->Focus();

			timer->Enabled = true;
			gamePaused = false;
			ApplySettings();
		}

		void ApplySettings() {
			if (settingsForm == nullptr) return;
			try {
				String^ leftColorText = settingsForm->leftColorBox->Text->Trim();
				if (!String::IsNullOrEmpty(leftColorText)) {
					leftPaddleColor = Color::FromName(leftColorText);
					if (leftPaddleColor.IsEmpty) {
						leftPaddleColor = ColorTranslator::FromHtml(leftColorText);
					}
				}
				String^ rightColorText = settingsForm->rightColorBox->Text->Trim();
				if (!String::IsNullOrEmpty(rightColorText)) {
					rightPaddleColor = Color::FromName(rightColorText);
					if (rightPaddleColor.IsEmpty) {
						rightPaddleColor = ColorTranslator::FromHtml(rightColorText);
					}
				}
				String^ ballColorText = settingsForm->ballColorBox->Text->Trim();
				if (!String::IsNullOrEmpty(ballColorText)) {
					ballColor = Color::FromName(ballColorText);
					if (ballColor.IsEmpty) {
						ballColor = ColorTranslator::FromHtml(ballColorText);
					}
				}
				paddle_height = settingsForm->leftSize->Value * 5;
				ball_size = settingsForm->ballSize->Value;
				String^ ballSpeedText = settingsForm->ballSpeedBox->Text->Trim();
				if (!String::IsNullOrEmpty(ballSpeedText)) {
					ball_speed = Convert::ToSingle(ballSpeedText);
				}
			}
			catch (...) {}
			this->Invalidate();
		}
#pragma endregion
	};
}
