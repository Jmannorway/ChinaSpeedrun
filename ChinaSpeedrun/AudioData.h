#pragma once

#include <vector>

namespace cs {
	/**
	 * A struct that holds metadata on a loaded audiofile
	 */
	struct AudioMeta
	{
		/**
		 *The length of the loaded sound in seconds
		 */
		float duration;
		/**
		 *Frequency of the loaded audio file (hz)
		 */
		unsigned rate;
		/**
		 *Bit depth of the loaded audio file
		 */
		unsigned depth;
		/**
		 *Number of channels of the loaded audio file (1 = mono, 2 = stereo, etc.)
		 */
		unsigned channels;
		AudioMeta(float duration, unsigned rate, unsigned depth, unsigned channels);
		AudioMeta() : duration(0.f), rate(0), depth(0), channels(0) {}
	};

	struct AudioData
	{
		AudioData(std::vector<uint8_t> buffer, AudioMeta meta);
		/**
		 * Loaded audio data in human readable format (if there is any)
		 */
		std::vector<float> pcm;
	private:
		/**
		 * Loaded sound as OpenAL compatible buffer data
		 */
		std::vector<uint8_t> buffer;
		/**
		 * Meta of the loaded sound data
		 */
		AudioMeta meta;
	};
}

