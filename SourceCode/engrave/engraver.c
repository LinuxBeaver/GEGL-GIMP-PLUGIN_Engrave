/* This file is an image processing operation for GEGL
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * 2006 Credit to Pippin
 * 2025 Beaver Engrave
* 
* Recreate plugin using this GEGL syntax
* 
local-threshold radius=34.0 aa-factor=9 low=0.5 high=1.0

over aux=[  checkerboard x=1000 y=4 color1=white color2=transparent 
ripple amplitude=45.0 period=344.0 phi=0 angle=-160  ] 
gamma value=1
color-overlay value=transparent

screen aux=[  color value=black ]


 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (threshold_radius, _("Threshold radius"), 34.0)
  description   (_("Radius of the black white threshold"))
  value_range   (1.0, 100.0)
  ui_steps      (0.5, 1.0)


property_int (line_size, _("Line's size"), 4)
  description   (_("Size of the lines"))
  value_range   (1, 25)
  ui_range   (1, 6)
  ui_steps      (1, 5)


property_double (line_amplitude, _("Line amplitude"), 34.0)
  description   (_("Ripple's amplitude makes waves out of straight lines"))
  value_range   (1.0, 100.0)
  ui_steps      (0.5, 1.0)


property_double (line_angle, _("Line angle"), -160.0)
  description   (_("Ripple's angle controls the angle of the waves"))
  value_range (-180, 180)
  ui_steps      (0.5, 1.0)
  ui_meta     ("unit", "degree")
  ui_meta     ("direction", "ccw")

property_double (line_phi, _("Line's progress through time"), -1.0)
  description   (_("Ripple's Phase Shift (phi) slider showing how the waving lines would change over time"))
  value_range   (-1.0, 1.0)
  ui_steps      (0.5, 1.0)

property_double (newsprint, _("Presence of newsprint dots"), 0.0)
  description   (_("Put newsprint dots on the engraved image"))
  value_range   (0.0, 1.0)
  ui_steps      (0.5, 1.0)


property_double (darken, _("Darken engraving"), 1.0)
  description   (_("Darken the engraved image"))
  value_range   (1.0, 10.0)
  ui_steps      (0.5, 1.0)


property_color  (color, _("Color"), "black")
  description   (_("The engraving's color (defaults to 'black')"))



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     engraver
#define GEGL_OP_C_SOURCE engraver.c

#include "gegl-op.h"

static void
attach (GeglOperation *operation)
{
  GeglNode *gegl   = operation->node;
  GeglColor *transparent = gegl_color_new ("rgba(0,0,0,0)");
  GeglColor *transparent2 = gegl_color_new ("rgba(0,0,0,0)");


  GeglColor *white = gegl_color_new ("#ffffff");


  GeglNode *output = gegl_node_get_output_proxy (gegl, "output");
  GeglNode *input  = gegl_node_get_input_proxy (gegl, "input");

  GeglNode *threshold  = gegl_node_new_child (gegl,
                                          "operation", "gegl:local-threshold",
                                          "radius", 34.0,
                                          "aa-factor", 9,
                                          "low", 0.5,
                                          "high", 1.0,
                                          NULL);
  GeglNode *normal  = gegl_node_new_child (gegl,
                                          "operation", "gegl:over",
                                          NULL);
  GeglNode *lines  = gegl_node_new_child (gegl,
                                          "operation", "gegl:checkerboard", "x", 100000, "y", 5, "color1", white, "color2", transparent,
                                          NULL);



  GeglNode *fix  = gegl_node_new_child (gegl,
                                          "operation", "gegl:color-overlay", "value", transparent2,
                                          NULL);
  GeglNode *color  = gegl_node_new_child (gegl,
                                          "operation", "gegl:color",
                                          NULL);
  GeglNode *crop  = gegl_node_new_child (gegl,
                                          "operation", "gegl:crop",
                                          NULL);
  GeglNode *screen  = gegl_node_new_child (gegl,
                                          "operation", "gegl:screen",
                                          NULL);
  GeglNode *gamma  = gegl_node_new_child (gegl,
                                          "operation", "gegl:gamma", "value", 2.0,
                                          NULL);
  GeglNode *alphalock  = gegl_node_new_child (gegl,
                                          "operation", "gegl:src-atop",    NULL);
  GeglNode *ripple  = gegl_node_new_child (gegl,
                                          "operation", "gegl:ripple", "amplitude", 45.0, "period", 344.0, "phi", 0.0, "angle", -160.0, 
                                          NULL);
  GeglNode *final  = gegl_node_new_child (gegl,
                                          "operation", "gegl:median-blur",
                                          "abyss_policy", 0,
                                          "radius", 0,
                                          NULL);
  GeglNode *newsprint  = gegl_node_new_child (gegl,
                                          "operation", "gegl:newsprint", "color-model", 0, "pattern", 1,    NULL);
  GeglNode *idref  = gegl_node_new_child (gegl,
                                          "operation", "gegl:nop",  NULL);
  GeglNode *srcatop  = gegl_node_new_child (gegl,
                                          "operation", "gegl:src-atop", NULL);
  GeglNode *opacity  = gegl_node_new_child (gegl,
                                          "operation", "gegl:opacity", "value", 0.0, NULL);

 GeglNode *opacity2  = gegl_node_new_child (gegl,
                                          "operation", "gegl:opacity", "value", 0.1, NULL);


/*the main graph is inside the gegl:src-atop blend mode which is like an alpha lock in GIMP*/
  gegl_node_link_many (input, alphalock, output, NULL);
/*the main graph that does the engraving effect is here. normal screen and srcatop are composers, technically gamma is a composer too but I am not using it that way.*/
  gegl_node_link_many (input, idref, srcatop,  threshold, normal, gamma, fix, screen, crop,  final, NULL);
  gegl_node_connect (alphalock, "aux", final, "output");
/*color overlay is blended by screen*/
  gegl_node_connect (screen, "aux", color, "output");
/*lines and ripple are a seperate normal virtual layer*/
  gegl_node_connect (normal, "aux", ripple, "output");
  gegl_node_link_many (lines, ripple, NULL);
/*srcatop connects to opacity*/
  gegl_node_connect (srcatop, "aux", opacity2, "output");
  gegl_node_link_many (idref, newsprint, opacity, opacity2, NULL);
/*crop is repairing the gegl graph, and it is technically a composer connecting to the original input */
  gegl_node_connect (crop, "aux", input, "output");

  gegl_operation_meta_redirect (operation, "line_amplitude", ripple, "amplitude"); 
  gegl_operation_meta_redirect (operation, "line_angle", ripple, "angle"); 
  gegl_operation_meta_redirect (operation, "line_phi", ripple, "phi"); 
  gegl_operation_meta_redirect (operation, "line_size", lines, "y");
  gegl_operation_meta_redirect (operation, "threshold_radius", threshold, "radius");            
  gegl_operation_meta_redirect (operation, "darken", gamma, "value");
  gegl_operation_meta_redirect (operation, "color", color, "value");
  gegl_operation_meta_redirect (operation, "newsprint", opacity, "value");

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_class->threaded = FALSE;

  gegl_operation_class_set_keys (operation_class,
    "name",           "lb:engrave",
    "title",          _("Engrave"),
    "reference-hash", "engravefuturegfunkisbased",
    "description",
    _("Pseudo engraves an image"),
    "gimp:menu-path", "<Image>/Filters/Artistic/",
    "gimp:menu-label", _("Engrave..."),
                                 NULL);
}

#endif
