#pragma once

/*
	Currently, when the AudioFile library reads an audio file it
	1) converts it from RAW buffer data into readable PCM data
	2) converts it back back to RAW buffer data
	The first step can be optimized away with further modifications to the AudioFile library
	(and will most likely be done later)
*/

#include <string>
#include <map>

#include "AudioData.h"

struct ALCdevice;
struct ALCcontext;

namespace cs {
	struct AudioBufferBlob {
		/**
		 * Maximum number of loaded audio files
		 */
		static constexpr unsigned max{ 10 };
		/**
		 * Current number of loaded audio files
		 */
		unsigned number;
		/**
		 * OpenAL indices for buffer data
		 */
		unsigned buffer[max];
		/**
		 * Meta for each loaded audio file
		 */
		AudioMeta meta[max];
		unsigned operator [](const unsigned i) const { return buffer[i]; }
		unsigned& operator [](const unsigned i) { return buffer[i]; }
		AudioBufferBlob();
	};

	struct AudioSourceBlob {
		/**
		 * Max number of sources that are allowed to be played at a time
		 */
		static constexpr unsigned max{ 100 };
		/**
		 * Index of the newest playing source
		 */
		unsigned index;
		/**
		 * OpenAL indices for sources
		 */
		unsigned source[max];
		unsigned operator [](unsigned i) const { return source[i]; }
		unsigned& operator [](unsigned i) { return source[i]; }
		AudioSourceBlob();
	};

	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();

		/**
		 * Updates an audio component
		 */
		void Update(class AudioComponent& ac);

		/**
		 * Initializes OpenAL and nulls buffer and source blobs
		 */
		void Init();
		/**
		 * Loads an audio file from a path
		 */
		bool Load(std::string path);
		/**
		 * Play a loaded audio file using its name
		 */
		unsigned Play(std::string name);
		/**
		 * Pause a playing audio file
		 */
		void Pause(unsigned sid);
		/**
		 * Stop a playing audio file
		 */
		void Stop(unsigned sid);
	private:

		std::map<std::string, unsigned> soundMap;
		AudioBufferBlob buffer;
		AudioSourceBlob source;
		ALCdevice* device;
		ALCcontext* context;
	};
}