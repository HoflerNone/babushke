
#pragma once
namespace PingPong { ref class GameField; }

namespace PingPong {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Text::RegularExpressions;

	/// <summary>
	/// Ñâîäêà äëÿ Settings
	/// </summary>
	public ref class Settings : public System::Windows::Forms::Form
	{
	public:
		Settings(void)
		{
			InitializeComponent();
			SetupEventHandlers();
		}

		bool SettingsChanged = false;
		Button^ btnApply;
		Button^ btnCancel;
		TextBox^ leftColorBox;
		TrackBar^ leftSize;
		Label^ leftText;
		GroupBox^ leftBox;
		GroupBox^ rightBox;
		Label^ rightText;
		TrackBar^ rightSize;
		TextBox^ rightColorBox;
		GroupBox^ ballBox;
		Label^ ballText;
		TrackBar^ ballSize;
		TextBox^ ballColorBox;
		TextBox^ ballSpeedBox;

	protected:
		/// <summary>
		/// Îñâîáîäèòü âñå èñïîëüçóåìûå ðåñóðñû.
		/// </summary>
		~Settings()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		/// <summary>
		/// Îáÿçàòåëüíàÿ ïåðåìåííàÿ êîíñòðóêòîðà.
		/// </summary>
		System::ComponentModel::Container^ components;

		/// <summary>
		/// Íàñòðîéêà îáðàáîò÷èêîâ ñîáûòèé äëÿ ïðîâåðêè ââîäà
		/// </summary>
		void SetupEventHandlers()
		{
			leftColorBox->TextChanged += gcnew EventHandler(this, &Settings::OnSettingChanged);
			rightColorBox->TextChanged += gcnew EventHandler(this, &Settings::OnSettingChanged);
			ballColorBox->TextChanged += gcnew EventHandler(this, &Settings::OnSettingChanged);
			ballSpeedBox->TextChanged += gcnew EventHandler(this, &Settings::OnSettingChanged);

			leftSize->Scroll += gcnew EventHandler(this, &Settings::OnSettingChanged);
			rightSize->Scroll += gcnew EventHandler(this, &Settings::OnSettingChanged);
			ballSize->Scroll += gcnew EventHandler(this, &Settings::OnSettingChanged);

			btnApply->Click += gcnew EventHandler(this, &Settings::btnApply_Click);
			btnCancel->Click += gcnew EventHandler(this, &Settings::btnCancel_Click);
		}

		void OnSettingChanged(Object^ sender, EventArgs^ e)
		{
			SettingsChanged = true;
			btnApply->Enabled = true;
		}

		void btnApply_Click(Object^ sender, EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}

		void btnCancel_Click(Object^ sender, EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

		/// <summary>
		/// Ïðîâåðêà ïîëåé öâåòà (èìÿ öâåòà èëè HEX)
		/// </summary>
		void ColorTextBox_TextChanged(System::Object^ sender, System::EventArgs^ e)
		{
			TextBox^ colorBox = safe_cast<TextBox^>(sender);
			String^ text = colorBox->Text->Trim();

			if (String::IsNullOrEmpty(text))
			{
				colorBox->BackColor = SystemColors::Window;
				return;
			}

			if (IsValidColor(text) || IsValidHexColor(text))
			{
				try
				{
					Color::FromName(text); // Äîïîëíèòåëüíàÿ ïðîâåðêà
					colorBox->BackColor = SystemColors::Window;
				}
				catch (...)
				{
					colorBox->BackColor = Color::LightPink;
				}
			}
			else
			{
				colorBox->BackColor = Color::LightPink;
			}
		}

		/// <summary>
		/// Ïðîâåðêà ñòàíäàðòíûõ èìåí öâåòîâ
		/// </summary>
		bool IsValidColor(String^ colorName)
		{
			// Îñíîâíûå öâåòà System::Drawing::Color
			array<String^>^ validColors = {
				L"Red", L"Green", L"Blue", L"Yellow", L"Magenta", L"Cyan", L"White", L"Black",
				L"Gray", L"LightGray", L"DarkGray", L"Orange", L"Purple", L"Brown", L"Pink",
				L"AliceBlue", L"AntiqueWhite", L"Aqua", L"Aquamarine", L"Azure", L"Beige",
				L"Bisque", L"BlanchedAlmond", L"BlueViolet", L"CadetBlue", L"Chartreuse",
				L"Chocolate", L"Coral", L"CornflowerBlue", L"Cornsilk", L"Crimson"
			};

			for each (String ^ color in validColors)
			{
				if (String::Compare(colorName, color, true) == 0)
					return true;
			}
			return false;
		}

		/// <summary>
		/// Ïðîâåðêà HEX ôîðìàòà (#RRGGBB èëè RRGGBB)
		/// </summary>
		bool IsValidHexColor(String^ hexColor)
		{
			Match^ match = Regex::Match(hexColor, L"^#?([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$");
			return match->Success;
		}

		/// <summary>
		/// Áëîêèðîâêà íåöèôðîâûõ ñèìâîëîâ â ïîëå ñêîðîñòè
		/// </summary>
		void BallSpeed_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
		{
			// Ðàçðåøàåì òîëüêî öèôðû 0-9, Backspace è Delete
			if (!Char::IsDigit(e->KeyChar) &&
				e->KeyChar != (Char)8 &&  // Backspace
				e->KeyChar != (Char)46)   // Delete
			{
				e->Handled = true;
			}
		}

		/// <summary>
		/// Ïðîâåðêà äèàïàçîíà ñêîðîñòè (0-100)
		/// </summary>
		void BallSpeed_TextChanged(System::Object^ sender, System::EventArgs^ e)
		{
			TextBox^ speedBox = safe_cast<TextBox^>(sender);
			String^ text = speedBox->Text->Trim();

			if (String::IsNullOrEmpty(text))
			{
				speedBox->BackColor = SystemColors::Window;
				return;
			}

			int speed;
			if (Int32::TryParse(text, speed) && speed >= 0 && speed <= 100)
			{
				speedBox->BackColor = SystemColors::Window;
			}
			else
			{
				speedBox->BackColor = Color::LightPink;
			}
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Òðåáóåìûé ìåòîä äëÿ ïîääåðæêè êîíñòðóêòîðà — íå èçìåíÿéòå 
		/// ñîäåðæèìîå ýòîãî ìåòîäà ñ ïîìîùüþ ðåäàêòîðà êîäà.
		/// </summary>
		void InitializeComponent(void)
		{
			this->leftColorBox = (gcnew System::Windows::Forms::TextBox());
			this->leftSize = (gcnew System::Windows::Forms::TrackBar());
			this->leftText = (gcnew System::Windows::Forms::Label());
			this->leftBox = (gcnew System::Windows::Forms::GroupBox());
			this->rightBox = (gcnew System::Windows::Forms::GroupBox());
			this->rightText = (gcnew System::Windows::Forms::Label());
			this->rightSize = (gcnew System::Windows::Forms::TrackBar());
			this->rightColorBox = (gcnew System::Windows::Forms::TextBox());
			this->ballBox = (gcnew System::Windows::Forms::GroupBox());
			this->ballText = (gcnew System::Windows::Forms::Label());
			this->ballSize = (gcnew System::Windows::Forms::TrackBar());
			this->ballColorBox = (gcnew System::Windows::Forms::TextBox());
			this->ballSpeedBox = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftSize))->BeginInit();
			this->leftBox->SuspendLayout();
			this->rightBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rightSize))->BeginInit();
			this->ballBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ballSize))->BeginInit();
			this->SuspendLayout();


			// BUTTONS
			// 
			// 

			this->btnApply = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());

			// 
			// leftColorBox
			// 
			this->leftColorBox->Location = System::Drawing::Point(43, 44);
			this->leftColorBox->Name = L"leftColorBox";
			this->leftColorBox->Size = System::Drawing::Size(148, 20);
			this->leftColorBox->TabIndex = 1;
			this->leftColorBox->Text = L"Black";
			// 
			// leftSize
			// 
			this->leftSize->Location = System::Drawing::Point(43, 94);
			this->leftSize->Minimum = 5;
			this->leftSize->Maximum = 30;
			this->leftSize->Value = 10;
			this->leftSize->Name = L"leftSize";
			this->leftSize->Size = System::Drawing::Size(147, 45);
			this->leftSize->TabIndex = 3;
			// 
			// leftText
			// 
			this->leftText->AutoSize = true;
			this->leftText->Location = System::Drawing::Point(83, 28);
			this->leftText->Name = L"leftText";
			this->leftText->Size = System::Drawing::Size(61, 13);
			this->leftText->TabIndex = 4;
			this->leftText->Text = L"Left Paddle";
			// 
			// leftBox
			// 
			this->leftBox->Controls->Add(this->leftText);
			this->leftBox->Controls->Add(this->leftSize);
			this->leftBox->Controls->Add(this->leftColorBox);
			this->leftBox->Location = System::Drawing::Point(3, 22);
			this->leftBox->Name = L"leftBox";
			this->leftBox->Size = System::Drawing::Size(224, 148);
			this->leftBox->TabIndex = 6;
			this->leftBox->TabStop = false;
			this->leftBox->Text = L"Left Paddle";
			// 
			// rightBox
			// 
			this->rightBox->Controls->Add(this->rightText);
			this->rightBox->Controls->Add(this->rightSize);
			this->rightBox->Controls->Add(this->rightColorBox);
			this->rightBox->Location = System::Drawing::Point(421, 22);
			this->rightBox->Name = L"rightBox";
			this->rightBox->Size = System::Drawing::Size(224, 148);
			this->rightBox->TabIndex = 8;
			this->rightBox->TabStop = false;
			this->rightBox->Text = L"Right Paddle";
			// 
			// rightText
			// 
			this->rightText->AutoSize = true;
			this->rightText->Location = System::Drawing::Point(78, 18);
			this->rightText->Name = L"rightText";
			this->rightText->Size = System::Drawing::Size(68, 13);
			this->rightText->TabIndex = 5;
			this->rightText->Text = L"Right Paddle";
			// 
			// rightSize
			// 
			this->rightSize->Location = System::Drawing::Point(43, 78);
			this->rightSize->Minimum = 5;
			this->rightSize->Value = 10;
			this->rightSize->Maximum = 30;
			this->rightSize->Name = L"rightSize";
			this->rightSize->Size = System::Drawing::Size(150, 45);
			this->rightSize->TabIndex = 2;
			// 
			// rightColorBox
			// 
			this->rightColorBox->Location = System::Drawing::Point(43, 34);
			this->rightColorBox->Name = L"rightColorBox";
			this->rightColorBox->Size = System::Drawing::Size(148, 20);
			this->rightColorBox->TabIndex = 0;
			this->rightColorBox->Text = L"Black";
			// 
			// ballBox
			// 
			this->ballBox->Controls->Add(this->ballSpeedBox);
			this->ballBox->Controls->Add(this->ballText);
			this->ballBox->Controls->Add(this->ballSize);
			this->ballBox->Controls->Add(this->ballColorBox);
			this->ballBox->Location = System::Drawing::Point(256, 104);
			this->ballBox->Name = L"ballBox";
			this->ballBox->Size = System::Drawing::Size(145, 210);
			this->ballBox->TabIndex = 10;
			this->ballBox->TabStop = false;
			this->ballBox->Text = L"Ball Settings";
			// 
			// ballText
			// 
			this->ballText->AutoSize = true;
			this->ballText->Location = System::Drawing::Point(55, 18);
			this->ballText->Name = L"ballText";
			this->ballText->Size = System::Drawing::Size(24, 13);
			this->ballText->TabIndex = 5;
			this->ballText->Text = L"Ball";
			// 
			// ballSize
			// 
			this->ballSize->LargeChange = 2;
			this->ballSize->Location = System::Drawing::Point(17, 79);
			this->ballSize->Minimum = 5;
			this->ballSize->Value = 10;
			this->ballSize->Maximum = 50;
			this->ballSize->Name = L"ballSize";
			this->ballSize->Size = System::Drawing::Size(102, 45);
			this->ballSize->TabIndex = 2;
			this->ballSize->Scroll += gcnew System::EventHandler(this, &Settings::trackBar5_Scroll);
			// 
			// ballColorBox
			// 
			this->ballColorBox->Location = System::Drawing::Point(17, 34);
			this->ballColorBox->Name = L"ballColorBox";
			this->ballColorBox->Size = System::Drawing::Size(109, 20);
			this->ballColorBox->TabIndex = 0;
			this->ballColorBox->Text = L"Red";
			// 
			// ballSpeedBox
			// 
			this->ballSpeedBox->Location = System::Drawing::Point(24, 142);
			this->ballSpeedBox->Name = L"ballSpeedBox";
			this->ballSpeedBox->Size = System::Drawing::Size(101, 20);
			this->ballSpeedBox->TabIndex = 6;
			this->ballSpeedBox->Text = L"5";
			// 
			// Settings
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(657, 462);
			this->Controls->Add(this->ballBox);
			this->Controls->Add(this->rightBox);
			this->Controls->Add(this->leftBox);
			this->Name = L"Settings";
			this->Text = L"PingPong - Settings";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftSize))->EndInit();
			this->leftBox->ResumeLayout(false);
			this->leftBox->PerformLayout();
			this->rightBox->ResumeLayout(false);
			this->rightBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rightSize))->EndInit();
			this->ballBox->ResumeLayout(false);
			this->ballBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ballSize))->EndInit();

			this->btnApply->Location = System::Drawing::Point(380, 410);
			this->btnApply->Size = System::Drawing::Size(120, 35);
			this->btnApply->Text = L"Apply";
			this->btnApply->Enabled = false;

			this->btnCancel->Location = System::Drawing::Point(520, 410);
			this->btnCancel->Size = System::Drawing::Size(120, 35);
			this->btnCancel->Text = L"Cancel";

			this->Controls->Add(this->btnApply);
			this->Controls->Add(this->btnCancel);

			this->ResumeLayout(false);
		}
#pragma endregion
	private: System::Void groupBox5_Enter(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void trackBar5_Scroll(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void groupBox2_Enter(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}