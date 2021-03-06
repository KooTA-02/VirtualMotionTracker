/*
MIT License

Copyright (c) 2020 gpsnmeajp

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "DirectOSC.h"
namespace DirectOSC {
	std::mutex Mutex;
	std::thread* Thread;

	void ThreadWorker()
	{
		printf("$waiting...\n");
		OSC::GetInstance()->GetSocketRx()->RunUntilSigInt();
		printf("$Done...\n");
	}

	OSC::OSC()
	{
	}
	OSC::~OSC()
	{
	}

	OSC* OSC::GetInstance()
	{
		static OSC osc;
		return &osc;
	}
	UdpListeningReceiveSocket* OSC::GetSocketRx()
	{
		return socketRx;
	}
	UdpTransmitSocket* OSC::GetSocketTx()
	{
		return socketTx;
	}
	void OSC::Open(osc::OscPacketListener* listen, int portRx,int portTx)
	{
		this->listener = listen;
		socketRx = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, portRx), listener);
		socketTx = new UdpTransmitSocket(IpEndpointName("127.0.0.1", portTx));

		Thread = new std::thread(ThreadWorker);
		m_opened = true;
	}
	void OSC::Close()
	{
		socketRx->AsynchronousBreak();
		Thread->join();

		delete socketRx;
		delete socketTx;
		delete Thread;
		socketRx = nullptr;
		socketTx = nullptr;
		Thread = nullptr;
		listener = nullptr;

		m_opened = false;
	}
}