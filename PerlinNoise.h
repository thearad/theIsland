#pragma once
#include <cmath>
/*
Utilized to create a height-map for procedural terrain.
http://stackoverflow.com/questions/4753055/perlin-noise-generation-for-terrain

amplitude = The maximum absolute value. Controls how high/low the terrain is.

frequency = Determines the frequency of bumps in the terrain

octaves = The number of octaves control the amount of detail of Perlin noise. 
Adding more octaves increases the detail of Perlin noise.

persistence = A multiplier that determines how quickly the amplitudes diminish 
for each successive octave

See: http://libnoise.sourceforge.net/glossary/ for detailed definitions of
each parameter
*/

class PerlinNoise
{
private:
	//Variables
	double persistence, frequency, amplitude;
	int octaves, randomseed;

	double Total(double i, double j) const;
	double GetValue(double x, double y) const;
	double Interpolate(double x, double y, double a) const;
	double Noise(int x, int y) const;
public:
	// Constructor
	PerlinNoise();
	PerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

	// Get Height
	double GetHeight(double x, double y) const;

	// Get
	double Persistence() const { return persistence; }
	double Frequency()   const { return frequency; }
	double Amplitude()   const { return amplitude; }
	int    Octaves()     const { return octaves; }
	int    RandomSeed()  const { return randomseed; }

	// Set
	void Set(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

	void SetPersistence(double _persistence) { persistence = _persistence; }
	void SetFrequency(double _frequency) { frequency = _frequency; }
	void SetAmplitude(double _amplitude) { amplitude = _amplitude; }
	void SetOctaves(int    _octaves) { octaves = _octaves; }
	void SetRandomSeed(int    _randomseed) { randomseed = _randomseed; }
};