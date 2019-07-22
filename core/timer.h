#pragma once

#pragma once

/**
*  High-precision timer for measuring frame-rate.
*/
class Timer
{
public:
	// Constructor
	Timer();

	// Start timer
	void Start();
	// Stop timer
	void Stop();
	// Take sample from running timer
	void Sample();

	// Get the elapsed seconds from the last sample to the previous sample
	double GetElapsedSeconds() const;
	// Get the elapsed microseconds from the last sample to the previous sample
	double GetElapsedMicroseconds() const;

private:
	LARGE_INTEGER m_oldTime;
	double m_ticksPerMicrosecond;
	double m_elapsedSeconds;
	double m_elapsedMicroseconds;
	bool m_isRunning;
};