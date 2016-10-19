#include <stdio.h>
#include <reconstructmesdk/reme.h>
#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

void aquire();
void read_file();
string check_ply_format(string filename);

int main() {

	string choice;
	choice[0] = 'A';
	
	while (choice[0] != '0')
	{
		cout << endl << "----\nMENU\n----" << endl << endl << "Please choose program choice : " << endl;
		cout << "1 - Acquire" << endl;
		cout << "2 - Read file" << endl;
		cout << "0 - Close" << endl;
		cin >> choice;

		switch (choice[0]) {
		case '1':
			aquire();
			break;
		case '2':
			read_file();
			break;
		}
	}

	

	return 0;

}

void aquire() {
	// Create a new context
	reme_context_t c;
	reme_context_create(&c);
	// Create options to set custom volume size
	reme_options_t o;
	reme_options_create(c, &o);
	reme_context_bind_reconstruction_options(c, o);

	// Set custom volume size
	reme_options_set_int(c, o, "volume.minimum_corner.x", -1000);
	reme_options_set_int(c, o, "volume.minimum_corner.y", -1000);
	reme_options_set_int(c, o, "volume.minimum_corner.z", -1000);

	reme_options_set_int(c, o, "volume.maximum_corner.x", 1000);
	reme_options_set_int(c, o, "volume.maximum_corner.y", 1000);
	reme_options_set_int(c, o, "volume.maximum_corner.z", 1000);

	reme_options_set_int(c, o, "volume.resolution.x", 128);
	reme_options_set_int(c, o, "volume.resolution.y", 128);
	reme_options_set_int(c, o, "volume.resolution.z", 128);

	// Tune reconstruction quality
	reme_context_tune_reconstruction_options(c, REME_TUNE_PROFILE_MID_QUALITY);

	reme_context_set_log_callback(c, reme_default_log_callback, 0);
	
	// Compile for OpenCL device using defaults
	reme_context_compile(c);
	// Create a new volume
	reme_volume_t v;
	reme_volume_create(c, &v);
	// Create a new sensor. Tries multiple backends using default
	// sensor configurations, returns the first working one. By default
	// each sensor works on the first volume created.
	reme_sensor_t s;
	reme_sensor_create(c, "openni;mskinect;file", true, &s);
	reme_sensor_open(c, s);
	reme_sensor_set_prescan_position(c, s, REME_SENSOR_POSITION_INFRONT);
	// For debugging purposes open a viewer for tracking the reconstruction process.
	reme_viewer_t viewer;
	reme_viewer_create_image(c, "This is ReconstructMe SDK", &viewer);
	reme_image_t volume, aux;
	reme_image_create(c, &volume);
	reme_image_create(c, &aux);
	reme_viewer_add_image(c, viewer, aux);
	reme_viewer_add_image(c, viewer, volume);
	// Reconstruct until viewer is closed, or grabbing fails. Note that
	// aborting reconstruction just because a single grab failed is not good stlye.
	// You should allow for multiple grab attempts.
	printf("Starting reconstruction. Close the viewer window to stop.\n");
	bool viewer_done = false;
	while (!viewer_done && REME_SUCCESS(reme_sensor_grab(c, s))) {
		// Prepare image and depth data
		reme_sensor_prepare_images(c, s);
		// Try to determine updated sensor position.
		// On succes, update volume, otherwise move to a recovery position
		// and wait for the tracking to start again.
		if (REME_SUCCESS(reme_sensor_track_position(c, s))) {
			// Update volume with depth data from the
			// current sensor perspective
			reme_sensor_update_volume(c, s);
		}
		// Update the viewer
		reme_sensor_get_image(c, s, REME_IMAGE_AUX, aux);
		reme_sensor_get_image(c, s, REME_IMAGE_VOLUME, volume);
		reme_viewer_update(c, viewer);
		reme_viewer_is_closed(c, viewer, &viewer_done);
	}
	// Close and destroy the sensor, it is not needed anymore
	reme_sensor_close(c, s);
	reme_sensor_destroy(c, &s);

	// ask for filename
	string filename;
	cout << "-- Give us filename : " << std::endl;
	cin >> filename;

	filename = check_ply_format(filename);

	// Create a new surface
	reme_surface_t m;
	reme_surface_create(c, &m);
	reme_surface_generate(c, m, v);
	reme_surface_save_to_file(c, m, filename.c_str());

	// Visualize resulting surface
	reme_viewer_t viewer_surface;
	reme_viewer_create_surface(c, m, "This is ReconstructMeSDK", &viewer_surface);
	reme_viewer_wait(c, viewer_surface);
	reme_surface_destroy(c, &m);
	// Print pending errors
	reme_context_print_errors(c);
	// Make sure to release all memory acquired
	reme_context_destroy(&c);
	
	return;
}

void read_file() {
	string filename;
	cout << "-- Which file to open " << endl;
	cin >> filename;

	filename = check_ply_format(filename);

	// Create a new context
	reme_context_t c;
	reme_context_create(&c);
	reme_context_set_log_callback(c, reme_default_log_callback, 0); 
	// Compile for OpenCL device using defaults
	reme_context_compile(c);
	// Create a new volume
	reme_volume_t v;
	reme_volume_create(c, &v);

	// Create a new surface
	reme_surface_t m;
	reme_surface_create(c, &m);
	reme_surface_generate(c, m, v);
	reme_surface_load_from_file(c, m, filename.c_str());

	// Visualize resulting surface
	reme_viewer_t viewer_surface;
	reme_viewer_create_surface(c, m, "This is ReconstructMeSDK", &viewer_surface);
	reme_viewer_wait(c, viewer_surface);
	reme_surface_destroy(c, &m);
	// Print pending errors
	reme_context_print_errors(c);
	// Make sure to release all memory acquired
	reme_context_destroy(&c);

	return;
}

inline bool ends_with(string const & value, string const & ending)
{
	if (ending.size() > value.size()) return false;
	return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

string check_ply_format(string filename)
{
	if (ends_with(filename, ".ply"))
		return filename;

	filename += ".ply";
	return filename;
}
