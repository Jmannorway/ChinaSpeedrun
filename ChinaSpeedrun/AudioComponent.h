#pragma once

#include "Component.h"

#include <string>

/**
 * Plays a loaded sound on command
 */
namespace cs
{
	class AudioSystem;

	class AudioComponent : public Component
	{
	public:

		friend AudioSystem;

		/**
		 * The name of the loaded sound that will play when 'play' is set to true
		 */
		std::string soundName;

		/**
		 * Id of the loaded sound. Will be used to stop, start and pause the playing sound
		 */
		unsigned soundId;

		/**
		 * Set to true when you want to play the sound
		 */
		bool play;
		/**
		 * Set to true when you want to stop the sound
		 */
		bool stop;
		/**
		 * Set if you want the sound to played at the transform
		 * of the game object this audio component belongs to
		 */
		bool onListener;

		AudioComponent();

		bool IsPlaying() const { return isPlaying; }
		float GetTime() const { return time; }
		float GetDuration() const { return duration; }
		void Init() override {}
		void ImGuiDrawComponent() override;

	private:
		/**
		 * Variable set by the system whether or not the sound is playing
		 */
		bool isPlaying;
		/**
		 * Time position in the currently playing sound
		 */
		float time;
		/**
		 * Total duration of the currently playing sound
		 */
		float duration;
	};
}
