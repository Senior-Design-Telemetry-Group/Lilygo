//Lilygo Interface Setup
//Version 2.0
//Use horizontal bars with scales to display data and add gradient colors 

#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>

LilyGo_Class amoled;

// UI Elements
lv_obj_t *speed_label, *rpm_label, *mpg_label;
lv_obj_t *speed_value, *rpm_value, *mpg_value;
lv_obj_t *speed_bar, *rpm_bar, *mpg_bar;

// Define text styles
static lv_style_t style_title, style_value, style_scale;

// Function to create a row with a gradient bar and scales
void create_dashboard_row(lv_obj_t *parent, int y, const char *title, lv_obj_t **value_label, lv_obj_t **bar, int min, int max) {
    // Create a horizontal container for the row
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, 600, 120);  
    lv_obj_align(row, LV_ALIGN_TOP_LEFT, 0, y);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(row, 5, 0);

    // Create title label (left side)
    lv_obj_t *title_label = lv_label_create(row);
    lv_label_set_text(title_label, title);
    lv_obj_set_width(title_label, 100);
    lv_obj_add_style(title_label, &style_title, 0);

    // Create numerical value label (middle)
    *value_label = lv_label_create(row);
    lv_label_set_text(*value_label, "0");
    lv_obj_set_width(*value_label, 80);
    lv_obj_add_style(*value_label, &style_value, 0);

    // Create a container for bar and scales
    lv_obj_t *bar_container = lv_obj_create(row);
    lv_obj_set_size(bar_container, 380, 90);
    lv_obj_set_style_border_width(bar_container, 0, 0);
    lv_obj_set_style_bg_opa(bar_container, LV_OPA_TRANSP, 0);
    lv_obj_align(bar_container, LV_ALIGN_CENTER, 0, 0);

    // Create the bar meter (right side)
    *bar = lv_bar_create(bar_container);
    lv_obj_set_size(*bar, 300, 30);
    lv_obj_align(*bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_bar_set_range(*bar, min, max);

    // Apply gradient style to the bar
    static lv_style_t style_bar;
    lv_style_init(&style_bar);
    lv_style_set_bg_color(&style_bar, lv_color_hex(0xFF2400));
    lv_style_set_bg_grad_color(&style_bar, lv_color_hex(0x00FF00));
    lv_style_set_bg_grad_dir(&style_bar, LV_GRAD_DIR_HOR);
    lv_obj_add_style(*bar, &style_bar, LV_PART_INDICATOR);

    // Add fine and coarse scale marks
    for (int i = 0; i <= 50; i++) {
        int x_offset = (i * 300) / 50 - 150; // Calculate position relative to the bar width

        lv_obj_t *tick = lv_obj_create(bar_container);
        lv_obj_set_size(tick, (i % 10 == 0) ? 3 : 2, (i % 10 == 0) ? 15 : 10); // Coarse: 15px, Fine: 10px
        lv_obj_align(tick, LV_ALIGN_TOP_MID, x_offset, 20); // Move ticks further below the bar
        lv_obj_set_style_bg_color(tick, lv_color_black(), 0);
        lv_obj_set_style_border_width(tick, 0, 0);

        // Add scale values for coarse marks
        if (i % 10 == 0) {
            lv_obj_t *scale_label = lv_label_create(bar_container);
            lv_label_set_text_fmt(scale_label, "%d", (max * i) / 50);
            lv_obj_align(scale_label, LV_ALIGN_BOTTOM_MID, x_offset, 0); // Lower scale numbers further
            lv_obj_add_style(scale_label, &style_scale, 0);
        }
    }

    // Create the bar meter inside the original container
    //*bar = lv_bar_create(row);
    //lv_obj_set_size(*bar, 300, 30);
    //lv_obj_align(*bar, LV_ALIGN_TOP_MID, 0, 0);
    //lv_bar_set_range(*bar, min, max);

    // Apply gradient style to the bar
    //static lv_style_t style_bar;
    //lv_style_init(&style_bar);
    //lv_style_set_bg_color(&style_bar, lv_color_hex(0xFF2400));
    //lv_style_set_bg_grad_color(&style_bar, lv_color_hex(0x00FF00));
    //lv_style_set_bg_grad_dir(&style_bar, LV_GRAD_DIR_HOR);
    //lv_obj_add_style(*bar, &style_bar, LV_PART_INDICATOR);
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

    // Initialize text styles
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_26);

    lv_style_init(&style_value);
    lv_style_set_text_font(&style_value, &lv_font_montserrat_28);

    lv_style_init(&style_scale);
    lv_style_set_text_font(&style_scale, &lv_font_montserrat_18);
    lv_style_set_text_color(&style_scale, lv_color_black());

    // Create three rows with gradient bars and scales
    create_dashboard_row(lv_scr_act(), 30, "Speed (km/h)", &speed_value, &speed_bar, 0, 200);
    create_dashboard_row(lv_scr_act(), 165, "RPM", &rpm_value, &rpm_bar, 0, 8000);
    create_dashboard_row(lv_scr_act(), 300, "MPG", &mpg_value, &mpg_bar, 0, 100);

    Serial.println("Setup completed.");
}

void loop() {
    lv_task_handler();

    // Generate random test values
    int speed = random(0, 200);
    int rpm = random(0, 8000);
    int mpg = random(0, 100);

    // Update labels
    lv_label_set_text_fmt(speed_value, "%d", speed);
    lv_label_set_text_fmt(rpm_value, "%d", rpm);
    lv_label_set_text_fmt(mpg_value, "%d", mpg);

    // Update bars with animation
    lv_bar_set_value(speed_bar, speed, LV_ANIM_ON);
    lv_bar_set_value(rpm_bar, rpm, LV_ANIM_ON);
    lv_bar_set_value(mpg_bar, mpg, LV_ANIM_ON);

    delay(33); // Update 30Hz
}
