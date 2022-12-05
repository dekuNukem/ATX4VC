# Pico ATX Adaptor for Macintosh 128K / 512K / Mac Plus

This board lets you use modern **Pico ATX PSU** in OG Macintosh computers.

It plugs into the motherboard power connector, and bypasses the unreliable built-in PSU all together.

## Features

* **Non-destructive** and reversible

* Easy assembly as **through-hole-only** kit

* Cheaper and simpler than recapping

* 4-Pin **PC fan header** for additional cooling

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

[Click me to jump to installation](#installation)

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

* Use plenty of additional flux, it makes everything much easier. You can clean it off afterwards.

* [This video](https://www.youtube.com/watch?v=AqvHogekDI4) covers the basics pretty well, you can also watch others by searching `how to solder`.

### Assembly

Note the top and bottom side of the PCB:

Insert the **3.96mm 11-Pin Female Header** on the BOTTOM side:

Hold it in place with masking tape or sticky putty. Flip it over, solder **a single pin** first, then make sure the component is straight in all directions.

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

### Finishing Touches

Push the fuse into the holder.

## Pre-installation Test

To make sure everything is working, I strongly suggest testing it out before installing.

* Plug in the Pico ATX PSU



## Installation