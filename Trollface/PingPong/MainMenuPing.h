#pragma once
#include "GameField.h"
#include "Settings.h"

namespace PingPong {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MainMenuPing : public System::Windows::Forms::Form
    {
    public:
        MainMenuPing(void)
        {
            InitializeComponent();
        }

    protected:
        ~MainMenuPing()
        {
            if (components)
                delete components;
        }

    private:
        System::ComponentModel::Container^ components;

    private: System::Windows::Forms::PictureBox^ background;
    private: System::Windows::Forms::Button^ gameButton;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Button^ obmanka;
    private: System::Windows::Forms::Button^ settings;
    private: System::Windows::Forms::Button^ button1;

           GameField^ gameForm;
           Settings^ settingsForm;

#pragma region Windows Form Designer generated code
           void InitializeComponent(void)
           {
               System::ComponentModel::ComponentResourceManager^ resources =
                   (gcnew System::ComponentModel::ComponentResourceManager(MainMenuPing::typeid));

               this->background = (gcnew System::Windows::Forms::PictureBox());
               this->gameButton = (gcnew System::Windows::Forms::Button());
               this->label1 = (gcnew System::Windows::Forms::Label());
               this->obmanka = (gcnew System::Windows::Forms::Button());
               this->settings = (gcnew System::Windows::Forms::Button());
               this->button1 = (gcnew System::Windows::Forms::Button());

               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->background))->BeginInit();
               this->SuspendLayout();

               this->background->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"background.BackgroundImage")));
               this->background->Location = System::Drawing::Point(-216, -11);
               this->background->Name = L"background";
               this->background->Size = System::Drawing::Size(1005, 560);
               this->background->TabIndex = 0;
               this->background->TabStop = false;

               this->gameButton->BackColor = System::Drawing::Color::Red;
               this->gameButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.25F));
               this->gameButton->Location = System::Drawing::Point(201, 304);
               this->gameButton->Name = L"gameButton";
               this->gameButton->Size = System::Drawing::Size(382, 89);
               this->gameButton->TabIndex = 1;
               this->gameButton->Text = L"START GAME";
               this->gameButton->UseVisualStyleBackColor = false;

               this->label1->AutoSize = true;
               this->label1->BackColor = System::Drawing::Color::CornflowerBlue;
               this->label1->Location = System::Drawing::Point(12, 21);
               this->label1->Name = L"label1";
               this->label1->Size = System::Drawing::Size(118, 13);
               this->label1->TabIndex = 2;
               this->label1->Text = L"Players in Online: 2.000";

               this->obmanka->BackColor = System::Drawing::Color::Black;
               this->obmanka->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"obmanka.BackgroundImage")));
               this->obmanka->Enabled = false;
               this->obmanka->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.25F));
               this->obmanka->ForeColor = System::Drawing::Color::FromArgb(224, 224, 224);
               this->obmanka->Location = System::Drawing::Point(228, 418);
               this->obmanka->Name = L"obmanka";
               this->obmanka->Size = System::Drawing::Size(331, 99);
               this->obmanka->TabIndex = 3;
               this->obmanka->Text = L"ONLINE GAME (IN WORKED)";

               this->settings->BackColor = System::Drawing::Color::Red;
               this->settings->Location = System::Drawing::Point(647, 12);
               this->settings->Name = L"settings";
               this->settings->Size = System::Drawing::Size(125, 32);
               this->settings->TabIndex = 4;
               this->settings->Text = L"SETTINGS";
               this->settings->UseVisualStyleBackColor = false;

               this->button1->BackColor = System::Drawing::SystemColors::MenuHighlight;
               this->button1->Location = System::Drawing::Point(15, 506);
               this->button1->Name = L"button1";
               this->button1->Size = System::Drawing::Size(92, 27);
               this->button1->TabIndex = 5;
               this->button1->Text = L"EXID";
               this->button1->UseVisualStyleBackColor = false;

               this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
               this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
               this->ClientSize = System::Drawing::Size(784, 561);
               this->Controls->Add(this->button1);
               this->Controls->Add(this->settings);
               this->Controls->Add(this->obmanka);
               this->Controls->Add(this->label1);
               this->Controls->Add(this->gameButton);
               this->Controls->Add(this->background);
               this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
               this->Name = L"MainMenuPing";
               this->Text = L"MainMenuPing";

               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->background))->EndInit();
               this->ResumeLayout(false);
               this->PerformLayout();

               this->gameButton->Click += gcnew System::EventHandler(this, &MainMenuPing::gameButton_Click);
               this->settings->Click += gcnew System::EventHandler(this, &MainMenuPing::settings_Click);
               this->button1->Click += gcnew System::EventHandler(this, &MainMenuPing::button1_Click);
           }
#pragma endregion

    private:
        System::Void gameButton_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (gameForm == nullptr || gameForm->IsDisposed)
                gameForm = gcnew GameField();

            LoadSettings(gameForm);

            gameForm->gamePaused = false;
            if (gameForm->overlayPanel != nullptr)
            {
                gameForm->overlayPanel->Visible = false;
                gameForm->overlayPanel->SendToBack();
            }
            if (gameForm->timer != nullptr)
                gameForm->timer->Enabled = true;

            this->Hide();
            gameForm->ShowDialog();
            this->Show();
        }
        System::Void settings_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (settingsForm == nullptr || settingsForm->IsDisposed)
                settingsForm = gcnew Settings();

            System::Windows::Forms::DialogResult result = settingsForm->ShowDialog();

            if (result == System::Windows::Forms::DialogResult::OK && settingsForm->SettingsChanged)
            {
                SaveSettings(settingsForm);

                if (gameForm != nullptr && !gameForm->IsDisposed)
                    LoadSettings(gameForm);
            }
        }
        System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
        {
            if (MessageBox::Show("Выйти из игры?", "Выход",
                MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes)
            {
                Application::Exit();
            }
        }

        void SaveSettings(Settings^ s)
        {
            try
            {
                System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter("settings.txt");

                writer->WriteLine(s->leftColorBox->Text);
                writer->WriteLine(s->rightColorBox->Text);
                writer->WriteLine(s->ballColorBox->Text);
                writer->WriteLine(s->leftSize->Value);
                writer->WriteLine(s->rightSize->Value);
                writer->WriteLine(s->ballSize->Value);
                writer->WriteLine(s->ballSpeedBox->Text);

                writer->Close();
            }
            catch (...) {}
        }

        void LoadSettings(GameField^ game)
        {
            if (game == nullptr) return;

            try
            {
                if (!System::IO::File::Exists("settings.txt")) return;

                System::IO::StreamReader^ reader = gcnew System::IO::StreamReader("settings.txt");

                game->SetLeftColor(reader->ReadLine());
                game->SetRightColor(reader->ReadLine());
                game->SetBallColor(reader->ReadLine());
                game->SetPaddleHeight(Convert::ToInt32(reader->ReadLine()) * 5);
                reader->ReadLine();
                game->SetBallSize(Convert::ToInt32(reader->ReadLine()));
                game->SetBallSpeed(Convert::ToSingle(reader->ReadLine()));

                reader->Close();
                game->Invalidate();
                game->initGame();
            }
            catch (...) {}
        }

    private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {}
    };
}