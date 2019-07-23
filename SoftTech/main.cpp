#include "VLP16Capture.h"
#include <iostream>
#include <math.h>

using namespace std;

int main()
{
	cout << "Program Starting" << endl;

	VelodyneVLP16PCAP capture;

	float last_x = 0;
	float last_y = 0;
	float last_z = 0;
	float last_dist = 0;
	int cycle_counter = 0;

	capture.open_live();

	if (!capture.isOpen()) {
		std::cerr << "Can't open the file." << std::endl;
		return -1;
	}

	while (capture.isRun()) {
		// Capture One Rotation Laser Data
		std::vector<DataPoint> dataPoints;
		capture.retrieve(dataPoints);
		if (dataPoints.empty()) {
			continue;
		}

		for (const DataPoint& laser : dataPoints) {
			const unsigned char id = laser.id; // Laser ID (VLP-16 : 0 - 15)
			const unsigned short distance = laser.distance; // Laser Distance ( centimeter )
			const unsigned char intensity = laser.intensity; // Laser Intensity
			const long long unixtime = laser.time; // Laser TimeStamp ( microseconds )
			const double azimuth = laser.azimuth; // Laser Azimuth ( degree )
			const double vertical = laser.vertical; // Laser Vertical ( degree )
			const float x = laser.coordinates.x; //X coordinate
			const float y = laser.coordinates.y; //Y coordinate
			const float z = laser.coordinates.z; //Z coordinate

			if (azimuth < 1 && azimuth > 0 && (id == 0 || id == 5 || id == 10 || id == 15))
			{
				if (id == 0)
				{
					cycle_counter++;
				}

				if (cycle_counter > 100)
				{
					cout << " distance: " << distance << " (" << static_cast<int>(id) << ")";
					if (id == 15)
					{
						cycle_counter = 0;
						cout << endl;
					}
				}

				/*if (abs(distance - last_dist) > 10.0f)
				{
					last_dist = distance;
				}*/

				/*if (sqrt((x - last_x) * (x - last_x) + (y - last_y) * (y - last_y) + (z - last_z) * (z - last_z)) > 2.0f)
				{
					last_x = x;
					last_y = y;
					last_z = z;
					cout << "x: " << x << "y: " << y << "z: " << z << endl;
				}*/
			}
		}
	}

	system("pause");
	return 0;
}