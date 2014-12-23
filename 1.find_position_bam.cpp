#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "api/BamReader.h"

#define type "WT"
//#define type "RGEN"

using namespace std;
using namespace BamTools;

int main(int argc, char** argv) {
    FILE *fsp = 0, *fep = 0;

	int flag;       // 0
	char chrom[255];    // 1
    int pos;        // 2
    int quality;    // 3
    char *cigar;    // 4

    size_t len = 0;
    
	char chromcopy[255] = { 0 };

	char fn[255];

	int i;
	int forward, reverse;

	bool direction; // 0
	bool leftflag;

	int cnt = 0;
    BamReader reader;

	reader.Open(argv[1]);
    const SamHeader header = reader.GetHeader();
    const RefVector references = reader.GetReferenceData();
    
    BamAlignment al;
    while(reader.GetNextAlignmentCore(al)) {
		flag = al.AlignmentFlag;
		if ((flag & 256) || (flag & 4) || (flag & 512) || (flag & 1024)) continue;

        direction = ( (flag & 16) == 16);
        strcpy(chrom, references[al.RefID].RefName.c_str());
        pos = al.Position+1;
        quality = al.MapQuality;
		if (quality == 0) continue;

		if (strcmp(chromcopy, chrom) != 0) {
			if (fsp) fclose(fsp);
			if (fep) fclose(fep);
			fn[0] = 0;
			strcat(fn, type);
			strcat(fn, "_");
			strcat(fn, chrom);
			strcat(fn, "_forward.txt");
			fsp = fopen(fn, "w");
			fn[0] = 0;
			strcat(fn, type);
			strcat(fn, "_");
			strcat(fn, chrom);
			strcat(fn, "_reverse.txt");
			fep = fopen(fn, "w");
			strcpy(chromcopy, chrom);
		}

        vector<CigarOp> cigars = al.CigarData;
		forward = pos;
		reverse = pos;
		leftflag = true;
		for (i = 0; i < cigars.size(); i++) {
			if (cigars[i].Type == 'S' || cigars[i].Type == 'H') {
				if (leftflag == false) break;
				continue;
			} else {
                if (cigars[i].Type != 'I') reverse += cigars[i].Length;
				leftflag = false;
			}
		}
		if (direction == 0)
			fprintf(fsp, "%d\n", forward);
		else
			fprintf(fep, "%d\n", reverse-1);

		cnt++;
		if (cnt % 100000 == 0)
			printf("%d\n", cnt);
	}
	reader.Close();
	if (fsp) fclose(fsp);
	if (fep) fclose(fep);
}