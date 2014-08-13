
#ifndef TIMEGM_H_
#define TIMEGM_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

time_t timegm(struct tm *tm);

#ifdef __cplusplus
}
#endif

#endif /* TIMEGM_H_ */
