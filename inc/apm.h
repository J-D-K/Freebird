#ifndef APM_H
#define APM_H

Result apmInit();
Result apmClose();

Result apmGetPerformanceMode(uint32_t *perf);

#endif // APM_H
