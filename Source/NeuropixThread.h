/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2017 Allen Institute for Brain Science and Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef __NEUROPIXTHREAD_H_2C4CBD67__
#define __NEUROPIXTHREAD_H_2C4CBD67__

#include <DataThreadHeaders.h>
#include <stdio.h>
#include <string.h>

#include "neuropix-api/Neuropix_basestation_api.h"
#include "neuropix-api/ElectrodePacket.h"

class SourceNode;

namespace Neuropix {

	/**

	  Communicates with imec version 3a Neuropixels probes
	  using a Kintex acquisition board

	  @see DataThread, SourceNode

	*/

	class NeuropixThread : public DataThread
	{

	public:

		/** Constructor */
	    NeuropixThread(SourceNode* sn);

		/** Destructor */
	    ~NeuropixThread();

		// DataThread Methods

		/** Static method for creating the DataThread object */
		static DataThread* createDataThread(SourceNode* sn);

		/** Creates the custom editor */
		std::unique_ptr<GenericEditor> createEditor(SourceNode* sn);

		/** Adds data to the AP and LFP band buffers*/
	    bool updateBuffer();

	    /** Returns true if the data source is connected, false otherwise.*/
	    bool foundInputSource();

		/** Called by ProcessorGraph to inform the thread whether the signal chain is loading */
		void initialize(bool signalChainIsLoading) override { }

	    /** Initializes data transfer.*/
	    bool startAcquisition() override;

	    /** Stops data transfer.*/
	    bool stopAcquisition() override;

		/** Update settings */
		void updateSettings(OwnedArray<ContinuousChannel>* continuousChannels,
			OwnedArray<EventChannel>* eventChannels,
			OwnedArray<SpikeChannel>* spikeChannels,
			OwnedArray<DataStream>* dataStreams,
			OwnedArray<DeviceInfo>* devices,
			OwnedArray<ConfigurationObject>* configurationObjects) override;

		// Neuropixels Methods
		/** Returns version info for hardware and API.*/
		void getInfo(String& hwVersion, String& bsVersion, String& apiVersion, String& asicInfo, String& serialNumber);


		/** Selects which electrode is connected to each channel. */
		void selectElectrode(int chNum, int connection, bool transmit);

		/** Selects which reference is used for each channel. */
		void setReference(int chNum, int refSetting);

		/** Selects which reference is used for each channel. */
		void setAllReferences(int refSetting, int bankForReference);

		/** Sets the gain for each channel. */
		void setGain(int ch, int apGain, int lfpGain);

		/** Sets the AP gains for all channels. **/
		void setAllApGains(int apGain);

		/** Sets the LFP gains for all channels. **/
		void setAllLfpGains(int lfpGain);

		/** Sets the filter for all channels. */
		void setFilter(int filter);

	    /** Toggles between internal and external triggering. */
	    void setTriggerMode(bool trigger);

		/** Toggles between auto-restart setting. */
		void setAutoRestart(bool restart);

		/** Loads all calibration settings stored on EEPROM. */
		void calibrateProbe();

		/** Loads ADC calibration settings stored on EEPROM. */
		void calibrateADCs();

		/** Loads gain calibration settings stored on EEPROM. */
		void calibrateGains();

		/** Loads calibration settings from CSV. */
		void calibrateFromCsv(File path);

		/** Retrieve probe option. */
		int getProbeOption();

		/** Turns AP data output on and off.*/
		void toggleApData(bool send);

		/** Turns LFP data output on and off.*/
		void toggleLfpData(bool send);

		CriticalSection* getMutex()
		{
			return &displayMutex;
		}

	    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuropixThread);

	private:
	    bool baseStationAvailable;
	    bool internalTrigger;
		bool autoRestart;
		bool sendAp;
		bool sendLfp;

		int counter;

		Neuropix_basestation_api neuropix;
		

		CriticalSection displayMutex;

		VersionNumber hw_version;
		unsigned char bs_version;
		unsigned char bs_revision;
		VersionNumber vn;
		AsicID asicId;

		Array<int> gains;
		Array<int> apGains;
		Array<int> lfpGains;
		Array<int> channelMap;
		Array<bool> outputOn;
		Array<int> refs;

		void openConnection();
		void closeConnection();

		int64 timestampAp;
		int64 timestampLfp;
		uint64 eventCode;
		int maxCounter;

		uint8 option;
		int numRefs;
		int totalChans;

		double timestamp_s = 0;

	};

};
#endif  // __NEUROPIXTHREAD_H_2C4CBD67__
