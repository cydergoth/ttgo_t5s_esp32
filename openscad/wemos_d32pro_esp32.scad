// all dimensions in mm
// https://docs.wemos.cc/en/latest/d32/d32_pro.html 

pcb_width = 65;
pcb_height = 25.4;
pcb_depth = 6;

pcb_clearance = 0.2;

// TODO: minkowski edges

module pcb_hole() {
  cylinder(h=8, d=2, center=true, $fn=32);
}

module d32pro_hull_void() { 
  difference() {
    translate([-pcb_clearance/2,-pcb_clearance/2,0]) {
      cube([pcb_height+pcb_clearance,pcb_width+pcb_clearance,pcb_depth]);
      }
          
    translate([2,2,4]) pcb_hole();
    translate([pcb_height-2,2,4]) pcb_hole(); 
  }
}

*d32pro_hull_void();

difference() {
  translate([-0.5,-0.5,-0.5]) cube([26.4,66,7]);
  d32pro_hull_void();
  translate([-pcb_clearance/2,-pcb_clearance/2,-2]) {
    cube([25.4+pcb_clearance,65+pcb_clearance,3]);
  }
}