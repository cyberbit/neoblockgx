//============================================================================
// Name        : NeoBlockGX.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include "NeoBlockBootup256.c"
#include "NeoBlockYouTube256.c"
#include "NeoBlockYouTubeTest256.c"
#include "NeoBLockJump256.c"

using namespace std;

void outputSafeHex() {
	for (int r = 0; r < 8; ++r) {
		for (int g = 0; g < 8; ++g) {
			for (int b = 0; b < 4; ++ b) {
				uint32_t color = (r << 21) | (g << 13) | (b << 6);
				cout << "0x" << hex << color << endl;
			}
		}
	}
}

template <size_t frames, size_t height, size_t width>
void readPiskelToCSV(const uint32_t (&x)[frames][height*width], const char *filename) {
	ofstream fout;
	fout.open(filename, ofstream::out);

	cout << "Writing CSV..." << endl;

	for (int i = 0; i < frames; ++i) {
		cout << "Frame " << dec << i << endl;

		for (int j = 0; j < height * width; ++j) {
			// Shift out alpha channel
//			uint32_t tmp = x[i][j] >> 8;

			// Pull out RGB components
			/**
			 * Lowercase bits move to uppercase location:
			 *
			 * 		Old method:
			 *		0       8       16
			 * 		rrr-------------RRR-----
			 *      --------ggg--------GGG--
			 *      ----------------bb----BB
			 *
			 *		New method:
			 *      0       8       16      24
			 *      ------------------------RRR-----	In this case, RRR is already in the right position
			 *      ----------------ggg--------GGG--
			 *      --------bb--------------------BB
			 */
//			int r = (tmp & 0b111000000000000000000000) >> 16;
//			int g = (tmp & 0b000000001110000000000000) >> 11;
//			int b = (tmp & 0b000000000000000011000000) >> 6;

			//                   AAAAAAAAbbBBBBBBgggGGGGGrrrRRRRR
			int r = (x[i][j] & 0b00000000000000000000000011100000);
			int g = (x[i][j] & 0b00000000000000001110000000000000) >> 11;
			int b = (x[i][j] & 0b00000000110000000000000000000000) >> 22;

			uint8_t color = r | g | b;

//			cout << "   Pixel " << dec << j << ": 0x" << hex << (int) color << endl;
			fout << dec << (int) color << (j == height * width - 1 ? "" : ",");
		}
		fout << endl;
	}

	cout << "Finished writing to " << filename << endl;

	fout.close();
}

int main() {
	outputSafeHex();

	// NeoBlockBootup256
	readPiskelToCSV<NEOBLOCKBOOTUP256_FRAME_COUNT,
		NEOBLOCKBOOTUP256_FRAME_HEIGHT,
		NEOBLOCKBOOTUP256_FRAME_WIDTH>
		(neoblockbootup256_data, "bootup.csv");

	// NeoBlockYouTube256
	readPiskelToCSV<NEOBLOCKYOUTUBE256_FRAME_COUNT,
		NEOBLOCKYOUTUBE256_FRAME_HEIGHT,
		NEOBLOCKYOUTUBE256_FRAME_WIDTH>
		(neoblockyoutube256_data, "youtube.csv");

	// NeoBlockYouTubeTest256
	readPiskelToCSV<NEOBLOCKYOUTUBETEST256_FRAME_COUNT,
		NEOBLOCKYOUTUBETEST256_FRAME_HEIGHT,
		NEOBLOCKYOUTUBETEST256_FRAME_WIDTH>
		(neoblockyoutubetest256_data, "youtubetest.csv");

	// NeoBlockJump256
	readPiskelToCSV<NEOBLOCKJUMP256_FRAME_COUNT,
		NEOBLOCKJUMP256_FRAME_HEIGHT,
		NEOBLOCKJUMP256_FRAME_WIDTH>
		(neoblockjump256_data, "jump.csv");

	return 0;
}
