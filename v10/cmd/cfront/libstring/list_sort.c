#include "list.h"

// binary counter - merge sort
// thanks to Alexander Stepanov
static const	logN = 32;	// max capacity will be 2^logN

void
voidP_list_sort_internal(voidP_list& aList, CMPF(voidP) lessThan)
{
	register voidPlnnk*	temp;
	register voidPlnnk*	newCh;
	register voidPlnnk*	oldCh;
	voidPlnnk*	bitPos[logN];
	voidPlnnk**	bitPtr;
	voidPlnnk**	bitPtrMax = &bitPos[0];
	for (bitPtr = &bitPos[0]; bitPtr < &bitPos[logN]; *bitPtr++ = 0) ;
	voidPlnnk* nextPtr = aList.head();
	aList.tail()->nxt = 0;
	voidPlnnk*	ans;
	while (newCh = nextPtr) {
		nextPtr = (voidPlnnk*)nextPtr->nxt;
		newCh->nxt = 0;
		for (bitPtr = &bitPos[0];; bitPtr++) {
			if (bitPtr > bitPtrMax) bitPtrMax = bitPtr;
			if (*bitPtr == 0) {
				*bitPtr = newCh;
				break;
			}
			oldCh = *bitPtr;
			*bitPtr = 0;
			if (!(*lessThan)(newCh->val, oldCh->val)) {
				ans = oldCh;
				for(;;) {
					while ((temp = (voidPlnnk*)oldCh->nxt) &&
					    !(*lessThan)(newCh->val, temp->val))
						oldCh = temp;
					oldCh->nxt = newCh;
					if ((oldCh = temp) == 0) {
						newCh = ans;
						break;
					}
bMerge:
					while ((temp = (voidPlnnk*)newCh->nxt) &&
					    (*lessThan)(temp->val, oldCh->val))
						newCh = temp;
					newCh->nxt = oldCh;
					if ((newCh = temp) == 0) {
						newCh = ans;
						break;
					}
				}
			} else {
				ans = newCh;
				goto bMerge;
			}
		}
	}
	// final merge sweep
	voidPlnnk**	bPtr2;
	for (bitPtr = &bitPos[0];; bitPtr = bPtr2) {
		while (*bitPtr == 0) bitPtr++;
		if (bitPtr == bitPtrMax) break;
		for (bPtr2 = bitPtr + 1; *bPtr2 == 0; bPtr2++) ;
		oldCh = *bPtr2;
		newCh = *bitPtr;
		if (!(*lessThan)(newCh->val, oldCh->val)) {
			ans = oldCh;
			for(;;) {
				while ((temp = (voidPlnnk*)oldCh->nxt) &&
				    !(*lessThan)(newCh->val, temp->val))
					oldCh = temp;
				oldCh->nxt = newCh;
				if ((oldCh = temp) == 0) {
					newCh = ans;
					break;
				}
eMerge:
				while ((temp = (voidPlnnk*)newCh->nxt) &&
				    (*lessThan)(temp->val, oldCh->val))
					newCh = temp;
				newCh->nxt = oldCh;
				if ((newCh = temp) == 0) {
					newCh = ans;
					break;
				}
			}
		} else {
			ans = newCh;
			goto eMerge;
		}
		*bPtr2 = ans;
	}
	for (newCh = *bitPtr; newCh->nxt; newCh = (voidPlnnk*)newCh->nxt)
		newCh->nxt->prv = newCh;
	newCh->nxt = *bitPtr;
	(*bitPtr)->prv = newCh;
	aList.it->t = newCh;
}

