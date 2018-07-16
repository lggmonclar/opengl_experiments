#pragma once

const double PI = 3.14;

double deg2rad(double deg) {
	return (deg * PI) / 180.0;
}

double rad2deg(double rad) {
	return (180.0 * rad) / PI;
}