/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de la tabla de descriptores globales
*/

#include "gdt.h"


gdt_entry_t gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] =
        {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x0000,
            .limit_19_16 = 0x00,
            .type = 0x0,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x00,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },
        //código nivel 0 y 3

        [GDT_IDX_CS_RING_0] =
        {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF,
            .limit_19_16 = 0x03,
            .type = 0x0A,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x01,
            .g = 0x01,
        },

        [GDT_IDX_CS_RING_3] =
        {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF,
            .limit_19_16 = 0x03,
            .type = 0x0A,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x01,
            .g = 0x01,
        },
        //datos nivel 0 y 3
        [GDT_IDX_DS_RING_0] =
        {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF,
            .limit_19_16 = 0x03,
            .type = 0x02,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
        },  

        [GDT_IDX_DS_RING_3] =
        {
            .base_15_0 = 0x0000,
            .base_23_16 = 0x00,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x30FF,
            .limit_19_16 = 0x03,
            .type = 0x2,
            .s = 0x01,
            .dpl = 0x03,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x01,
        }, 

        [GDT_IDX_DS_RING_0_VIDEO] =
        {
            .base_15_0 = 0x8000,
            .base_23_16 = 0x0B,
            .base_31_24 = 0x00,
            .limit_15_0 = 0x1F3F,
            .limit_19_16 = 0x00,
            .type = 0x02,
            .s = 0x01,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x1,
            .g = 0x00,
        },
        [GDT_IDX_TSS_TAREA_INICIAL] =
        {
            .base_15_0 = 0,
            .base_23_16 = 0,
            .base_31_24 = 0,
            .limit_15_0 = 0x67,
            .limit_19_16 = 0x00,
            .type = 0x09,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },
        [GDT_IDX_TSS_TAREA_IDLE] =
        {
            .base_15_0 = 0,
            .base_23_16 = 0,
            .base_31_24 = 0,
            .limit_15_0 = 0x67,
            .limit_19_16 = 0x00,
            .type = 0x09,
            .s = 0x00,
            .dpl = 0x00,
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },
        //acá empiezan las tareas de los lemmings
        [GDT_IDX_TSS_TAREAS_BASE] =
        {
            .base_15_0 = 0,
            .base_23_16 = 0,
            .base_31_24 = 0,
            .limit_15_0 = 0x67,
            .limit_19_16 = 0x00,
            .type = 0x09,
            .s = 0x00,
            .dpl = 0x00, 
            .p = 0x01,
            .avl = 0x0,
            .l = 0x0,
            .db = 0x0,
            .g = 0x00,
        },
};
        



gdt_descriptor_t GDT_DESC = {sizeof(gdt) - 1, (uint32_t)&gdt};

