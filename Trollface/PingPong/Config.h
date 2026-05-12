#pragma once
#include "Settings.h"

namespace PingPong {

    public ref class Config
    {
    public:
        static void Save(Settings^ settingsForm)
        {
            try
            {
                System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter("settings.cfg");

                sw->WriteLine("left_color=" + settingsForm->leftColorBox->Text);
                sw->WriteLine("right_color=" + settingsForm->rightColorBox->Text);
                sw->WriteLine("ball_color=" + settingsForm->ballColorBox->Text);
                sw->WriteLine("left_size=" + settingsForm->leftSize->Value);
                sw->WriteLine("right_size=" + settingsForm->rightSize->Value);
                sw->WriteLine("ball_size=" + settingsForm->ballSize->Value);
                sw->WriteLine("ball_speed=" + settingsForm->ballSpeedBox->Text);

                sw->Close();
            }
            catch (...) {}
        }

        static void Load(GameField^ game)
        {
            if (game == nullptr) return;

            try
            {
                if (!System::IO::File::Exists("settings.cfg"))
                    return;

                System::IO::StreamReader^ sr = gcnew System::IO::StreamReader("settings.cfg");
                String^ line;

                while ((line = sr->ReadLine()) != nullptr)
                {
                    array<String^>^ parts = line->Split('=');
                    if (parts->Length != 2) continue;

                    String^ key = parts[0]->Trim();
                    String^ value = parts[1]->Trim();

                    if (key == "left_color")   game->SetLeftColor(value);
                    if (key == "right_color")  game->SetRightColor(value);
                    if (key == "ball_color")   game->SetBallColor(value);

                    if (key == "left_size" || key == "right_size")
                        game->SetPaddleHeight(Convert::ToInt32(value) * 5);

                    if (key == "ball_size")
                        game->SetBallSize(Convert::ToInt32(value));

                    if (key == "ball_speed")
                    {
                        try { game->SetBallSpeed(Convert::ToSingle(value)); }
                        catch (...) {}
                    }
                }
                sr->Close();
                game->Invalidate();
                game->initGame();
            }
            catch (...) {}
        }
    };
}