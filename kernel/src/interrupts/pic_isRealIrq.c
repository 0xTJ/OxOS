#include "pic.h"

bool picIsNormalIrq(uint8_t irqNum) {
	switch (irqNum) {
	case 7:
		if ((picGetIsr() >> 7) & 1)
			return 1;
		else
			return 0;
	case 15:
		if ((picGetIsr() >> 15) & 1)
			return 1;
		else
			return 0;
	default:
		return 1;
	}
}
