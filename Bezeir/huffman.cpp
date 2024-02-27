
#include "huffman.h"
#include "list.h"

#include "toolutil.h"
#include "log.h"
VOID HuffTest() {
	LPCSTR ssss = "fff";
	int iiiii = 0x00650066;
	_log.Trace("heee %d %s", ssss,&iiiii);

	VOID* buffer;
	int iblen,i,j,iused,inewlen; 
	HNODE* root;
	//List<HNODE> hasic(CREATE_STACKLIST(HNODE,512),TRUE);
	BYTE b;
	HNODE hasic[256];
	LPCSTR str = "{'name':'jquery','main':'dist/jquery.js','license':'MIT','ignore':['package.json'],'keywords':['jquery','javascript','browser','library'],'homepage':'https://github.com/jquery/jquery-dist','version':'3.3.1','_release':'3.3.1','_resolution':{'type':'version','tag':'3.3.1','commit':'9e8ec3d10fad04748176144f108d7355662ae75e'},'_source':'https://github.com/jquery/jquery-dist.git','_target':'^3.3.1','_originalSource':'jquery','_direct':true}";
	//memset(hasic, 0, sizeof(HNODE) * 256);
	for (i = 0; i < 256; i++) {
		hasic[i].ascii = i;
	}
	iblen = HuffBuffCreate(&buffer, str);
	for (i = 0; i < iblen; i++) {
		b = ((BYTE*)buffer)[i];
		++hasic[b].frequency;
	}
	iused=HuffNodeSort(hasic);
	inewlen = (iused + (iused - 1));
	HNODE* newHNode = (HNODE*)malloc(inewlen * sizeof(HNODE));
	ASSERT(newHNode==NULL);
	memset(newHNode, 0, inewlen * sizeof(HNODE));
	memcpy(newHNode, hasic, sizeof(HNODE) * iused);
	HuffRemake(newHNode, hasic, iused);
	UINT outhuff[50000] = {0};
	int itotalHuffBitsSize ;
	BYTE decodeBuffer[50000] = { 0 };
	itotalHuffBitsSize =GetHuffCodeArray(newHNode, iused, (BYTE*)str, strlen(str), outhuff);
	HuffDecode(newHNode, iused, outhuff, itotalHuffBitsSize, decodeBuffer);
	for (i = 0; i < strlen(str); i++) {

		ShowHuffCode(newHNode, inewlen, str[i]);

	}

}

VOID HuffDecode(HNODE huffNode[],int inodeLen, UINT huffCodes[], int itotalBits, BYTE decodeBuffer[]) {
	UINT code, bitmask;
	int i, j, k,iibitsSize=sizeof(int)<<3,ibitIndex,codePos,itotalNodeLen= (inodeLen<<1)-1
		,decPos, itmptotalBits,ibitshift;
	BYTE bit;
	HNODE cnode, rootNode = {0}; 
	itmptotalBits = itotalBits;
	ibitIndex = 0;
	codePos = 0;
	decPos = 0;
	cnode = huffNode[0];
	for (i = 0;; i++) {
		if (cnode.parentIndex == 0) {
			rootNode = cnode;
			break;
		}
		cnode = huffNode[cnode.parentIndex];
	} 
	ASSERT(rootNode.frequency == 0);
	cnode = rootNode;
	i = 0;
	for (; ; ) {
		code= huffCodes[codePos];
		ibitshift = (iibitsSize - 1 - i);
		bitmask = 1 << ibitshift;
		bit = (code & bitmask) >> ibitshift;
		if (bit == 1) {
			cnode = huffNode[cnode.rightIndex];
		}
		else {
			cnode = huffNode[cnode.leftIndex];
		}
		if (cnode.index < inodeLen) {
			decodeBuffer[decPos++] = cnode.ascii;
			cnode = rootNode;
		}
		++i;
		if (i == iibitsSize) {
			i = 0;
			++codePos;
		}
		++ibitIndex;
		if (ibitIndex == itotalBits) {
			break;
		}
	}



}


VOID HuffRemake(HNODE newNode[], HNODE srcNode[], int len) {
	HNODE tmpNode1, tmpNode2, tmpParent = { 0 };
	int i, j,k,inewLen= (len + (len - 1)),iIndex,ileft,ifreq1,ifreq2,iright,iusedNew=0,iparent= len,iparentPos=0,idep=log2(inewLen);

	for (;iparent< inewLen ;) {
		ifreq1 = 0;
		ifreq2 = 0;
		for (j = 0; j < inewLen; j++) {
			tmpNode1 = newNode[j];
			if (tmpNode1.frequency == 0) {
				continue;
			}
			if (tmpNode1.parentIndex == 0) {
				if (ifreq1 == 0) {
					ifreq1 = tmpNode1.frequency;
					ileft = j;
				}else if (ifreq1 > tmpNode1.frequency) {
					ifreq1 = tmpNode1.frequency;
					ileft = j;
				}
			} 
		}
		for (j = 0; j < inewLen; j++) {
			tmpNode2 = newNode[j];
			if (tmpNode2.frequency == 0) {
				continue;
			}
			if (tmpNode2.parentIndex == 0&& j != ileft) {
				if ( ifreq2 == 0  ) {
					ifreq2 = tmpNode2.frequency;
					iright = j;
				}
				else  if ( ifreq2 > tmpNode2.frequency) {
						ifreq2 = tmpNode2.frequency;
						iright = j;
					} 
			}
		}


		++iusedNew;
		newNode[iparent].frequency= newNode[ileft].frequency + newNode[iright].frequency;
		newNode[iparent].leftIndex = ileft;
		newNode[iparent].rightIndex = iright;
		newNode[iparent].index = iparent;
		newNode[ileft].parentIndex = iparent;
		newNode[ileft].index = ileft;
		newNode[iright].parentIndex = iparent;
		newNode[iright].index = iright;
		 ++iparent;

	}

}


USHORT GetHuffCode(HNODE* hnode, int ilen, CHAR c) {
	HNODE htmp = { 0 }, hparent = { 1 }, hleft;
	int i, j,bitmask=0;
	USHORT res=0;
	for (i = 0; i < ilen; i++) {
		htmp = hnode[i];
		if (htmp.ascii == c) {
			break;
		}
		htmp = { 0 };
	}
	if (htmp.frequency == 0) {
		return -1;
	}
	j = 0; 
	hparent = hnode[htmp.parentIndex]; bitmask = 0;
	for (; hparent.parentIndex != 0;) {
		hparent = hnode[htmp.parentIndex];
		hleft = hnode[hparent.leftIndex];
		if (hleft.index == htmp.index) {
			bitmask = 0  ;
		}
		else {
			bitmask = 1 ;
		}
		htmp = hparent;
		++j;
		res |=  bitmask;
		res = res << 1;
	}
	j = j + 1;
	res = NumBitReflex(res, j);
	bitmask = j << 12;
	res |= bitmask;
	return res;
}

int GetHuffCodeArray(HNODE* hnode, int ilen, BYTE* bytes, int bytesLen, UINT code[]){
	HNODE htmp = { 0 }, hparent = { 1 }, hleft;
	int i, j,k,n,codePos=0,ibitssize=sizeof(int)*8,ibitshift=0,itotal=0, itmpCodePos;
	USHORT res = 0;
	BYTE c,bit;
	UINT tmpCode[5] = { 0 },itmpTotalbits, tmpnum,itmpbitshift,itmpmask, bitmask;

	CHAR testc[100] = { 0 };
	ibitshift = 0;
	itotal = 0;
	bitmask = 0;
	for (n= 0; n < bytesLen; n++) {
		c = bytes[n];
		for (i = 0; i < ilen; i++) {
			htmp = hnode[i];
			if (htmp.ascii == c) {
				break;
			}
			htmp = { 0 };
		}
		if (htmp.frequency == 0) {
			return -1;
		}
		memset(tmpCode, 0, sizeof(tmpCode));
		itmpCodePos = 0;
		itmpTotalbits = 0;
		hparent = hnode[htmp.parentIndex];
		bitmask = 0;
		j = 0;
		for (; hparent.parentIndex != 0;) {
			hparent = hnode[htmp.parentIndex];
			hleft = hnode[hparent.leftIndex];
			if (hleft.index == htmp.index) {
				bitmask = 0;
				testc[j] = '0';
			}
			else {
				bitmask = 1;
				testc[j] = '1';
			}
			htmp = hparent; 
			ibitshift = ibitssize - j - 1;
			 bitmask=bitmask<<(ibitshift); 
			 tmpCode[itmpCodePos] |= bitmask;
			 ++itmpTotalbits;
			 ++j;
			 if (j == ibitssize) {
				 j = 0;
				++itmpCodePos;
			 }
		} 
		j = 0;

		for (;  ; ) {
			tmpnum = tmpCode[itmpCodePos];
			itmpmask = itmpTotalbits % ibitssize;
			itmpbitshift = ibitssize-(itmpmask);
			itmpbitshift = itmpbitshift == ibitssize ? 0 : itmpbitshift;
		
			for (k = 0; k < itmpmask; k++) {
				bit = (tmpnum & (1 << (itmpbitshift + j))) >> (itmpbitshift + j);
				j++;
				if ((j + itmpbitshift) == ibitssize) {
					j = 0;
					itmpTotalbits -= bitmask;
					--itmpCodePos;
				}
				ibitshift = ibitssize - (itotal % ibitssize) - 1;
				bitmask = bit << ibitshift;
				code[codePos] |= bitmask; 
				++itotal;
				if (itotal % ibitssize == 0) {
					++codePos;
				}
			} 
			if (itmpCodePos < 0) {
				break;
		}
		}
	


	} 
	 

	return itotal;
}
VOID ShowHuffCode(HNODE hnode[],int ilen, CHAR c) {
	HNODE htmp = { 0 }, hparent = {0}, hleft;
	int i, j,bitlen;
	CHAR outcons[100] = {0}, tmpc;
	USHORT us;
	us = GetHuffCode(hnode, ilen, c);
	bitlen = (us & 0xf000)>>12;
	us = us & 0x0fff;
	NumToBits(us, bitlen, outcons);
	j = bitlen;

	outcons[j] = ':';
	outcons[j + 1] = c;
	outcons[j + 2] = '\n';
	OutputDebugStringA(outcons);

}
int HuffNodeSort(HNODE  hroot[]) {
	int i, j,freq, nIndex =-1,iused=256;

	HNODE  n1,n2,tmpn;
	for (i = 0; i < 256; i++) {
		HNODE& tmpNode = hroot[i];
		if (nIndex != -1) {
			if (tmpNode.frequency !=0) { 
				hroot[nIndex] = tmpNode;
				hroot[i] = { 0 };
				++nIndex;
			}
			else {
				--iused;
			}
		}
		if (tmpNode.frequency==0) {
			if (nIndex == -1) {
				nIndex = i;
				--iused;
			}
		}
	}
	for (i = 0; i < iused; i++) {
		n1 = hroot[i];
		for (j = i + 1; j < iused; j++) {
			n2 = hroot[j];
			if (n1.frequency > n2.frequency) {
				tmpn = hroot[i];
				hroot[i] = hroot[j];
				hroot[j] = tmpn;
				n1 = hroot[i];
			}

		}

	}
	return iused;
}
int HuffBuffCreate(VOID** obuffer,LPCSTR str) {
	int iblen = 1 << 16, istrlen;
	VOID* buffer;
	*obuffer = malloc(iblen);
	buffer = *obuffer;
	istrlen = strlen(str);
	memcpy(buffer, str, istrlen);
	return istrlen;
}
