#include "VLP16Capture.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;
using Frame = vector<Coordinates>;

int main()
{
	cout << "Program Starting" << endl;
	
	VelodyneVLP16PCAP capture;

	Frame current_frame = Frame();

	float last_azimuth = 0;
	int cycle_counter = 0;
	int frame_counter = 0;

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

			if (last_azimuth - azimuth > 300) //we just turned over the 360 degree mark
			{
				//we should copy the current frame and pass it to the drawer class
				ofstream outputFile;
				// create a name for the file output
				string filename = "exampleOutput" + to_string(frame_counter) + ".csv";

				// create and open the .csv file
				outputFile.open(filename);
				outputFile << "x,y,z" << endl;
				for (const Coordinates& coord : current_frame)
				{
					outputFile << to_string(coord.x) << "," << to_string(coord.y) << "," << to_string(coord.z) << endl;
				}
				outputFile.close();
				frame_counter++;
				current_frame = Frame();
			}
			current_frame.push_back(laser.coordinates);
			last_azimuth = azimuth;
					   			 
			if (azimuth < 1 && azimuth > 0 && (id == 0 || id == 7 || id == 15))
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
			}
		}
	}

	system("pause");
	return 0;
}