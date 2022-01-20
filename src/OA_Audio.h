#ifndef __OA_AUDIO_H__
#define __OA_AUDIO_H__

#include <string>
#include <map>

namespace onart {
	class Audio
	{
	public:
		static void init();
	private:
		std::map<std::string, int> source;
		
	};
}

#endif // !__OA_AUDIO_H__
