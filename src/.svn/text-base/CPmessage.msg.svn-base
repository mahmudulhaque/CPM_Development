packet CPmessage
{
    int src;
    int dest;

    string cpm;
    bool rollingback = false;
    bool mainflow = true;
    bool interpreted = false;
    bool processed = false;
    bool failed = false;

    simtime_t startTime = 0.0;
    simtime_t procTime = 0.0;
    simtime_t interpretTime = 0.0;
    simtime_t queueTime = 0.0;
    simtime_t queueDelay = 0.0;
    simtime_t skew = 0.0;
    simtime_t commTime = 0.0;
    simtime_t failLastDetectTime = 0.0;
    simtime_t rollbackStartTime = 0.0;
	int numRepeats = 0;
    int numSites = 0;
    int sites[100];
    int numSitesVisited = 0;
    int sitesVisited[100];
}
