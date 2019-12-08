#include "RtAudio.h"
#include "ofMain.h"

//------------------------------------------------------------------------------
RtAudio::Api ofToRtAudio(ofSoundDevice::Api api)
{
    switch (api) {
    case ofSoundDevice::Api::ALSA:
        return RtAudio::Api::LINUX_ALSA;
    case ofSoundDevice::Api::PULSE:
        return RtAudio::Api::LINUX_PULSE;
    case ofSoundDevice::Api::OSS:
        return RtAudio::Api::LINUX_OSS;
    case ofSoundDevice::Api::JACK:
        return RtAudio::Api::UNIX_JACK;
#ifndef TARGET_LINUX
    case ofSoundDevice::Api::OSX_CORE:
        return RtAudio::Api::MACOSX_CORE;
    case ofSoundDevice::Api::MS_WASAPI:
        return RtAudio::Api::WINDOWS_WASAPI;
    case ofSoundDevice::Api::MS_ASIO:
        return RtAudio::Api::WINDOWS_ASIO;
    case ofSoundDevice::Api::MS_DS:
        return RtAudio::Api::WINDOWS_DS;
#endif
    default:
        return RtAudio::Api::UNSPECIFIED;
    }
}

template<typename T> string padRight(T t, const int& width, const char& separator = ' ')
{
    stringstream ss; ss << left << setw(width) << setfill(separator) << t;
	return ss.str();
}

//========================================================================
int main()
{
    ofSoundStream soundstream;

    std::vector<RtAudio::Api> apis;
    RtAudio::getCompiledApi(apis);

	cout	<< padRight("AUDIO DRIVER", 20) << "| " << padRight("DEVICE", 60) << "| DETAILS\n"
			<< padRight("------------", 20) << "| " << padRight("------", 60) << "| -------\n";

    for (int i = ofSoundDevice::ALSA; i < ofSoundDevice::NUM_APIS; ++i) {
        ofSoundDevice::Api api = (ofSoundDevice::Api)i;
        if (std::find(apis.begin(), apis.end(), ofToRtAudio(api)) != apis.end()) {
            for (auto& device : soundstream.getDeviceList(api)) {
                cout	<< padRight(toString(api), 20) << "| "		// driver
						<< padRight(device.name, 60) << "| "		// device
																	// details:
						<< "in: " << padRight(device.inputChannels, 3) << (device.isDefaultInput ? "* | " : "  | ") 
						<< "out: " << padRight(device.outputChannels, 3) << (device.isDefaultOutput ? "* | " : "  | ")
						<< "rates: ";
				for (auto& rate : device.sampleRates) {
					cout << rate << " ";
				}
				cout << "\n";
            }
        }
    }
}
