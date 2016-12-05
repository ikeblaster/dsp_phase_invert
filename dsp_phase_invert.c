// Winamp test dsp library 0.9 for Winamp 2
// Copyright (C) 1997, Justin Frankel/Nullsoft
// Feel free to base any plugins on this "framework"...

#include <windows.h>
#include "../Winamp/dsp.h"


// module getter.
winampDSPModule *getModule(int which);

void config(struct winampDSPModule *this_mod);
int init(struct winampDSPModule *this_mod);
void quit(struct winampDSPModule *this_mod);
int modify_phase_invert(struct winampDSPModule *this_mod, short int *samples, int numsamples, int bps, int nch, int srate);


// Module header, includes version, description, and address of the module retriever function
typedef struct {
	int version;       // DSP_HDRVER
	char *description; // description of library
	winampDSPModule* (*getModule)(int);	// module retrieval function
	int(*sf)(int);
} winampDSPHeaderEx;

int sf(int v)
{
	int res;
	res = v * (unsigned long) 1103515245;
	res += (unsigned long) 13293;
	res &= (unsigned long) 0x7FFFFFFF;
	res ^= v;
	return res;
}

winampDSPHeaderEx hdr = { DSP_HDRVER + 1, "Phase invert DSP", getModule, sf };

// first module
winampDSPModule mod =
{
	"Phase invert",
	NULL,	// hwndParent
	NULL,	// hDllInstance
	config,
	init,
	modify_phase_invert,
	quit
};


// this is the only exported symbol. returns our main header.
__declspec(dllexport) winampDSPHeaderEx *winampDSPGetHeader2()
{
	return &hdr;
}

// getmodule routine from the main header. Returns NULL if an invalid module was requested,
// otherwise returns either mod1 or mod2 depending on 'which'.
winampDSPModule *getModule(int which)
{
	switch (which)
	{
		case 0:
			return &mod;
		default:
			return NULL;
	}
}

// configuration. Passed this_mod, as a "this" parameter. Allows you to make one configuration
// function that shares code for all your modules (you don't HAVE to use it though, you can make
// config1(), config2(), etc...)
void config(struct winampDSPModule *this_mod)
{
	//MessageBox(this_mod->hwndParent, "", "Configuration", MB_OK);
}

int init(struct winampDSPModule *this_mod)
{
	return 0;
}
// cleanup (opposite of init()). Destroys the window, unregisters the window class
void quit(struct winampDSPModule *this_mod)
{
}


int modify_phase_invert(struct winampDSPModule *this_mod, short int *samples, int numsamples, int bps, int nch, int srate)
{
	for (int n = 0; n < numsamples; n++)
	{
		for (int ch = 0; ch < nch; ch++)
		{
			if (ch == 0)
				samples[0] = -samples[0];

			samples++;
		}
	}

	return numsamples;
}
