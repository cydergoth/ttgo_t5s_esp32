// all dimensions in mm
// https://www.pjrc.com/store/display_ili9341_touch.html

screen_height = 50;
screen_width = 69.2;
screen_depth = 4;
screen_clearance = 0.6;

pcb_height = 52;
pcb_width = 87;
pcb_depth = 6;

screen_offset_width = 1;
screen_offset_height = 6.4;

hole_clearance=1.0;

// TODO: minkowski the top edges

module pcb_hole() {
  cylinder(h=pcb_depth+screen_depth+4,d=3.5-hole_clearance, center=true, $fn=32);
}

module pcb_standoff() {
  cylinder(h=screen_depth+0.1,d=5.2, center=true, $fn=32);
}

module display_ili9341_hull_void() { 
  difference() {
    union() {
      translate([screen_offset_height-0.3, screen_offset_width-0.3,0]) {
        cube([screen_width+0.6,screen_height+0.6,screen_depth + screen_clearance]);
      }
      translate([0,0,-pcb_depth]) {
        cube([pcb_width,pcb_height,pcb_depth]);
      }
      // Additional clearance above PCB beyond screen end
      translate([0,1,0]) cube([screen_offset_height, pcb_height-2,screen_depth-1]);
      translate([pcb_width-10.4,1,0]) cube([10.4, pcb_height-2,screen_depth-1]);
    }
    translate([3,3.5,-pcb_depth/2]) pcb_hole();
    translate([pcb_width-6.92,3.5,-pcb_depth/2]) pcb_hole();
    translate([pcb_width-6.92,pcb_height-5+0.5,-pcb_depth/2]) pcb_hole();
    translate([3,pcb_height-5+0.5,-pcb_depth/2]) pcb_hole();
    
    translate([3,3.5,screen_depth/2+0.1]) pcb_standoff();
    translate([pcb_width-6.92,3.5,screen_depth/2+0.1]) pcb_standoff();
    translate([pcb_width-6.92,pcb_height-5+0.5,screen_depth/2+0.1]) pcb_standoff();
    translate([3,pcb_height-5+0.5,screen_depth/2+0.1]) pcb_standoff();
  }  
}

union() { 
  difference()  {
     translate([-0.5,-0.5,3-pcb_depth]) { 
         minkowski() {
           cube([pcb_width+1,pcb_height+1,(pcb_depth+screen_depth)/2-1]);
           cylinder(r=1,h=(pcb_depth+screen_depth)/2, center=true, $fn=32);
         }
       }
     display_ili9341_hull_void();
    }
  }
  
*display_ili9341_hull_void();
