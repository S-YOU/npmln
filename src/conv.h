#ifndef RC_CONV_H_
#define RC_CONV_H_

//unsigned mul; str sTmp;
#define ATOI(BUF, BUF_END, NUM) \
	NUM = 0, mul = 1, sTmp = BUF_END - 1;\
	while (sTmp >= BUF) {\
		NUM += (*sTmp - '0') * mul;\
		mul *= 10;\
		sTmp--;\
	}\
	if (*sTmp == '-') NUM = ~NUM, NUM++;

//chr uTmp[20]; str p; unsigned vTmp;
//append to d
#define ITOA(nTmp) \
	vTmp = nTmp, p = uTmp;\
	do *p++ = (vTmp % 10) + '0', vTmp /= 10; while (vTmp > 0);\
	do *d++ = *--p; while (p != uTmp);

#endif
