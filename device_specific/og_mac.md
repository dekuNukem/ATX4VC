# ATX4VC Pico for Macintosh 128K / 512K / Mac Plus

This adaptor lets you use modern **Pico ATX PSU** in OG Macintosh computers.

It plugs into the motherboard power connector, and bypasses the unreliable built-in PSU all together.

## Features

* **Non-destructive** and reversible

* Easy assembly as **through-hole-only** kit

* Cheaper and simpler than recapping

* **PC fan header** for additional cooling

* **Fused** positive rails

* Power switch and battery-backup headers

* Less heat, stable picture, reliable operation.

## Repair Notes

If your Mac is not working, this adaptor eliminates the PSU from the equation, but that's only part of the puzzle!

Other common issues include:

* Cracked solder joints

* Shorted tantalum capacitors

* Dirty contacts / Bad RAMs

I strongly recommend watching [Adrian's Digital Basement Macintosh Repair-a-thon](https://www.youtube.com/watch?v=lKD65I86XGQ), where he covers a lot of those issues in detail.

## Pico ATX PSU

You need one of those. They are tiny ATX power supplies for small form-factor PCs, but are perfect for retro computers as well!

Couple of things to note:

* Just search `Pico ATX` on ebay/amazon

* I tried many different brands and they all work fine, but try get one with beefier components.

* Many claim a power output of 160W, which I find highly dubious. Half of that is more realistic, still plenty though.

* They require a power brick with **12V DC** with **center positive** 5.5x2.1mm barrel jacks. Those are very popular and you might already have one. Double check the label.

## Kit Assembly

### Already assembled?

[Click me to jump to next section](#board-information) (Board Information).

### Required tools

* **TEMPERATURE CONTROLLED** Soldering Iron

* Solder (leaded 60/40 or 63/37 recommended)

* Flux

* Multimeter

* Wire cutter

### Parts

By now, you should have:

### Soldering Notes

Nothing too tricky in this kit, all basic through-hole stuff.

If this is your first time, a few tips:

* Make sure your soldering iron has proper **temperature control**, NOT the cheap ones with just a power switch! Those get way too hot and will damage the PCB very easily.

* Use **leaded solder**, much easier to work with.

* Set the temperature around 320C / 600F

* Use plenty of additional flux, makes everything much easier. You can clean it off afterwards.

* [This video](https://www.youtube.com/watch?v=AqvHogekDI4) covers the basics pretty well, you can also watch others by searching `how to solder`.

### Assembly

Note the top and bottom side of the PCB:

Insert the **3.96mm 11-Pin Female Header** on the BOTTOM side:

Hold it in place with masking tape or sticky putty. Flip it over, solder **a single pin** first, then make sure the component is **straight in all directions**.

If not, you can adjust it while melting that single pin. Once happy, solder the rest of the pins. **This applies to all components in this kit.**

Now you can do the same with the rest of the parts on the TOP side. 

I would suggest starting from shortest component to tallest. That would be fuse holder -> 0.1 inch headers -> terminal block -> fan header -> ATX connector -> 11-pin power connector.

Observe the notes:

fuse holder metal part all the way into plastic base.

### Inspection

* Solder joints should be shiny and smooth. If you see spikes, put on more flux and melt it again.

* There must be no solder bridges. If any, put on flux and melt it to remove.

* Make sure the solder melts properly and wets both the pin and pad to prevent cold joints, especially on thicker power pins.

### Cleaning

This is optional, but I like to clean off the flux with 90%+ isopropyl alcohol. Submerge and scrub with a toothbrush.

Make sure it is **completely dry** before proceeding.

## Board Information

### Connectors

Battery header

Power Switch header and terminal block

4-pin PC fan header

You can run the PC fan at either 5V or 12V. At 12V it will spin at full speed, while 5V will be much quieter.

### Fuse Info

* Use common car fuses RATED **5A OR LESS**

* Regular, Mini, and low-profile Mini all will work. Simply push into the holder.

* **DO NOT BYPASS FUSES**

## Pre-installation Test

Use a multimeter to **check for dead shorts** between each power pin and GND


If all good, plug in Pico ATX PSU, and the barrel jack connector.

The PSU should turn on, measure the voltage on each rail and confirm they are correct and within spec, careful with the metal probe to prevent shorts.

## Installation

During disassembly, it's a good idea to **Take plenty of photos**. It never hurts to have reference.

### Remove Back Cover

⚠️**Dangerous CRT voltage inside**! Make sure the computer is **unplugged and unused for a few hours**! Or discharge the CRT yourself.

You'll need a long **Torx T15 screwdriver**.

Recommended steps:

* Lay the Mac face down on a soft cloth

* Loosen all the screws. Two at the bottom, one behind battery door, two in the handles.

* Take photos of which goes in where, they might be different!

* Remove all the screws **apart from a single one inside the handle**.

* Grab the handle with one hand, put the screwdriver on that single loosened screw, and try **gently** lifting the handle up. The back cover should pop loose.

* Slowly **lift straight up** to remove the back cover.

### Inspect and Clean

While we're in, might as well give it an once-over.

Possible activities:

* Blow out dust

* [Service the floppy drive](https://www.youtube.com/watch?v=0pGhwtyFG2I)

* Inspect the analog board.

* Remove the paper cover by pulling out the plastic pins. Look for damaged parts and cracked solder joints.

* Common location include power connectors and CRT capacitors near the top.

* The signs might be very subtle, so magnification might be needed.

* Again, see [Adrian's Mac Repair-a-thon](https://www.youtube.com/watch?v=lKD65I86XGQ) for details.

* **DO NOT TOUCH CRT COMPONENTS** if you don't know what you're doing!

### Install the Adaptor

* Unplug the power connector from motherboard.

* **DO NOT PULL STRAIGHT UP**, or you might break the CRT neck!

* Wiggle gently until loose, and carefully unplug.

* Plug Pico ATX PSU into the adaptor, make sure the power switch jumper is installed.

* Plug the adaptor into the motherboard power connector.

* It should go in firmly, although not reach the bottom.

* **Bend down and double check** the pins are lined up and NOT off-by-one!

* Plug analog board power cable into the adaptor.

* Double check for shorts again if you want, you can do it by removing the paper cover on the analog board.

### Moment of Truth!

If everything checks out, take a breath and plug in the barrel jack. Hopefully it will beep, and everything still works!

If so, we're pretty much done! If not, time to start troubleshooting!

Measure the voltage again, check fuse, and then go through the usual repair steps. Should be plenty of resources online.

### Big Mac Fan

Those early macs run really hot, and putting in a fan would help in prolonging the lifetime.

Any PC fan should work, you can select 5V or 12V, I like to set it to 5V so it's quieter.

picture of fan with jumpers and point out the cap spot.

Some fans running on 12V might generate interference with CRT, try move it further away, or add some decoupling caps on the 12V line. One spot is available on the adaptor.

### Power Cable and Switch

I'll admit, this is