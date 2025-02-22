//Lilygo Interface Setup
//Version 1.0
//Use three meters represent data 

#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>

LilyGo_Class amoled;

// UI Elements
lv_obj_t *speed_meter, *rpm_meter, *fuel_meter;
lv_meter_indicator_t *needle_speed, *needle_rpm, *needle_fuel;

// Function to create a dashboard with a moving pointer
void create_dashboard(lv_obj_t *parent, int x, int y, int min, int max, const char *title, lv_obj_t **meter, lv_meter_indicator_t **needle) {
    // Create the meter
    *meter = lv_meter_create(parent);
    lv_obj_set_size(*meter, 200, 200);
    lv_obj_align(*meter, LV_ALIGN_TOP_LEFT, x, y);

    // Add a scale to the meter
    lv_meter_scale_t *scale = lv_meter_add_scale(*meter);
    lv_meter_set_scale_range(*meter, scale, min, max, 300, 120); // 300° arc at 120° offset
    lv_meter_set_scale_ticks(*meter, scale, 51, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(*meter, scale, 10, 4, 15, lv_color_black(), 10);

    // Add a moving needle indicator
    *needle = lv_meter_add_needle_line(*meter, scale, 4, lv_palette_main(LV_PALETTE_RED), -10);

    // Create label for the dashboard title
    lv_obj_t *title_label = lv_label_create(*meter);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_BOTTOM_MID, 0, -100);

    // Create label for numerical value
    //lv_obj_t *value_label = lv_label_create(meter);
    //lv_obj_align(value_label, LV_ALIGN_TOP_MID, 0, -10); // Align above the center
    //lv_label_set_text(value_label, "0");
}

void setup() {
    Serial.begin(115200);

    if (!amoled.begin()) {
        while (1) {
            Serial.println("Board model not detected!");
            delay(1000);
        }
    }

    beginLvglHelper(amoled);

    // Create three dashboards with dynamic needles
    create_dashboard(lv_scr_act(), 30, 220, 0, 200, "km/h", &speed_meter, &needle_speed);
    create_dashboard(lv_scr_act(), 370, 220, 0, 8000, "RPM", &rpm_meter, &needle_rpm);
    create_dashboard(lv_scr_act(), 200, 30, 0, 300, "MPH", &fuel_meter, &needle_fuel);

    Serial.println("Setup completed.");
}

void loop() {
    lv_task_handler();

    // Generate random test values
    int speed = random(0, 200);
    int rpm = random(0, 8000);
    int fuel = random(0, 100);

    // Update labels
    //lv_label_set_text_fmt(label_speed, "%d km/h", speed);
    //lv_label_set_text_fmt(label_rpm, "%d RPM", rpm);
    //lv_label_set_text_fmt(label_fuel, "%d% MPH", fuel);

    //lv_obj_align(label_speed, LV_ALIGN_TOP_MID, 0, -50);
    //lv_obj_align(label_rpm, LV_ALIGN_TOP_MID, 0, -50);
    //lv_obj_align(label_fuel, LV_ALIGN_TOP_MID, 0, -50);

    // Update needle positions
    lv_meter_set_indicator_value(speed_meter, needle_speed, speed);
    lv_meter_set_indicator_value(rpm_meter, needle_rpm, rpm);
    lv_meter_set_indicator_value(fuel_meter, needle_fuel, fuel);

    delay(500); // Update every 500ms
}
