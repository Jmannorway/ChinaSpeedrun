#include "AudioComponent.h"

#include "imgui.h"

cs::AudioComponent::AudioComponent() : soundId{ 0 }, play{ false }, stop{ false }, isPlaying{ false }, onListener{ false }, time{ 0 }, duration{ 0 }
{
	type = AUDIO_COMPONENT_TYPE;
}

void cs::AudioComponent::ImGuiDrawComponent()
{
	if (ImGui::TreeNodeEx("Audio", ImGuiTreeNodeFlags_DefaultOpen))
	{
		char buf[128];
		strncpy_s(buf, soundName.c_str(), sizeof(buf) - 1);
		ImGui::InputText("Sound Name", &buf[0], sizeof(buf));
		soundName = buf;

		ImGui::Checkbox("Play", &play);
		ImGui::SameLine();
		ImGui::Checkbox("Stop", &stop);
		ImGui::SameLine();
		ImGui::ProgressBar(time / duration);

		ImGui::TreePop();
	}
}
