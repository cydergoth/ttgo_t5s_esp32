// All dimensions mm

// NOTE: These locations and sizes have not been verified!

board_w=46.96;
board_h=81.31;
board_d=8;
board_pcb_d=1.5;

m_hole_d = 3.00;
m_hole_r_x = 2.58;  // From right edge of board
m_hole_x = 2.48; // From left edge of board
m_hole_t_y = 2.89; // From top edge of board
m_hole_y = 2.58; // From botttom edge of board

epaper_standoff=3.00; // Height above board of e-paper
epaper_w=board_w;
epaper_h=board_h-5.5 // Top edge
     -5.5; // Bottom edge

// ePaper cable notch
notch_w=19;
notch_h=5;
notch_x=5;

// .1" expansion connector
point_one_x=6;
point_one_y=31;
point_one_h=21;
point_one_w=5;
point_one_d=10.5;

// Microusb
usb_x=31;
usb_y=-2.0;
usb_h=6.0;
usb_w=8.5;
usb_d=4;

// Grove expansion connector
grove_w=6;
grove_h=12;
grove_d=5.5;
grove_x=board_w-grove_w;
grove_y=41;

// WiFi and BT antenna
antenna_w=14;
antenna_h=5.5;
antenna_d=4;
antenna_y=board_h-8-antenna_h;
antenna_x=board_w-antenna_w-2;

// SD/TF Card slot
sd_card_w=14;
sd_card_h=15;
sd_card_d=2;
sd_card_x=0;
sd_card_y=board_h-12-sd_card_h;

// Battery connector
bat_w=7;
bat_h=9;
bat_d=4;
bat_x=39;
bat_y=18;

// MEMS microphones
mic_w=2;
mic_h=3;
mic_d=1;

mic_1_x=25.5;
mic_1_y=0;

mic_2_x=22;
mic_2_y=board_h-mic_h;

union() {
     case();
     !translate([0,0,2]) {
	  ttgo_t5s();
     }
};

module case() {
     difference() {
	  cube([80,90,8], center=true); // case external
	  translate([0,0,3]) {
	       // epaper window
	       cube([epaper_w+0.4,epaper_h+0.4,3], center=true);
	  }
	  translate([0,0,-1.5]) {
	       cube([board_w+2,board_h+2,6], center=true);
	  }
     }
}

// loc = offset of bottom left of component corner of board
// dim = dimensions of component
// flip = use non-component side of board
module component(loc,dim, flip=false) {
     if(flip) {
	  mirror([0,0,1])
	       translate([-board_w/2+loc.x,-board_h/2+loc.y,+board_pcb_d/2+loc.z]) {
	       cube(dim);
	  }
     } else {
	  translate([-board_w/2+loc.x,-board_h/2+loc.y,+board_pcb_d/2+loc.z]) {
	       cube(dim);
	  }
     }
}

module switch(loc) {
     translate([-board_w/2+loc.x,-board_h/2+loc.y,+board_pcb_d/2+loc.z]) {
	  union() {
	       cube([3,4.5,2.0]); // Body
	       translate([-1.5/2,1.25,0.6]) {
		    cube([1.5,2.0,0.8]);
	       }
	  }
     }
}

module ttgo_t5s() {
     // Main board. Reference point is mid plane of PCB
     // Board orientation: Component side up, e-paper tab at bottom
     union() {
	  translate([0,0,-board_pcb_d/2]) {
	       //PCB
	       difference() {
		    cube([board_w, board_h, board_pcb_d], center=true);
		    // Mounting hole bottom left
		    translate([-board_w/2+m_hole_x,-board_h/2+m_hole_y,0]) {
			 cylinder(board_pcb_d+0.2, d=m_hole_d, center=true, $fn=30);
		    }
		    // Mounting hole bottom right
		    translate([-board_w/2+m_hole_x,board_h/2-m_hole_t_y,0]) {
			 cylinder(board_pcb_d+0.2, d=m_hole_d, center=true, $fn=30);
		    }
		    // Mounting hole top left
		    translate([board_w/2-m_hole_r_x,-board_h/2+m_hole_y,0]) {
			 cylinder(board_pcb_d+0.2, d=m_hole_d, center=true, $fn=30);
		    }
		    // Mounting hole top right
		    translate([board_w/2-m_hole_r_x,board_h/2-m_hole_t_y,0]) {
			 cylinder(board_pcb_d+0.2, d=m_hole_d, center=true, $fn=30);
		    }
		    // Notch
		    component([notch_x,0,0-board_pcb_d],[ notch_w,notch_h,board_pcb_d+0.4]);
	       }
	       component([0,5.5,0],[epaper_w,epaper_h, epaper_standoff], flip=true);
	       component([point_one_x, point_one_y, 0],[point_one_w, point_one_h, point_one_d]);
	       component([usb_x, usb_y, 0],[usb_w, usb_h, usb_d]);
	       component([grove_x, grove_y, 0], [grove_w, grove_h, grove_d]);
	       component([antenna_x, antenna_y, 0], [antenna_w, antenna_h, antenna_d]);
	       component([sd_card_x, sd_card_y, 0], [sd_card_w, sd_card_h, sd_card_d]);
	       component([bat_x, bat_y, 0], [bat_w, bat_h, bat_d]);
	       component([mic_1_x, mic_1_y, 0], [mic_w, mic_h, mic_d]);
	       component([mic_2_x, mic_2_y, 0], [mic_w, mic_h, mic_d]);

	       switch([0,25,0]);
	       switch([0,32,0]);
	       switch([0,39,0]);
	       switch([0,46,0]);

	  }
     }
}
