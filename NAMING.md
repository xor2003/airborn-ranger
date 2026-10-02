# Semantic function naming

Decompiled names are evidence-driven, not guesswork. A wrong name is worse than
no name — it makes every call site lie.

## The map

`tools/names.map` is the single source of truth. One line per rename:

```
oldname  newname  [tnd]   # evidence note
```

- untagged lines rename AR.EXE functions (`lifted/ar.exe*.c`,
  `port/gen/ar.exe*.c`, `procs.h`, `lifted_procs.h`, `memimg.c` fmap,
  `rt_tracef` probes, `gen_port.py` TRACE_PROCS).
- `tnd` lines rename TANDYSND.EXE overlay procs; the port applies the `tnd_`
  prefix itself (`tnd_timer_isr`, `tw_tnd_timer_isr` fmap wrappers).
- `#` comments carry the evidence that earned the name. Keep them — they are
  the audit trail.

Apply: `python3 tools/rename.py` (idempotent, in-place on maintained sources).

## Rules

1. **Body evidence beats caller context.** Name what the function *does*, not
   where it is called. `word_1d94d = ax; spin-until-zero` → `delay_ticks`.
   If the body doesn't prove it, keep `sub_XXXX`.
2. **Evidence ladder** (strongest first): body behavior → referenced strings →
   API/BIOS/DOS calls → record constants/dispatch cases → call-graph position.
3. **Honest names.** `compose_frame_cond` (same body, extra gate) is honest;
   guessing "vsync_handler" is not. Structural classifications are fine:
   `timer_isr`, `patched_trampoline`, `farcall_ptr_a9e`.
4. **Propagate everywhere.** `tools/rename.py` covers definitions, calls,
   decls, fmap entries, `tw_tnd_*` wrappers, and `rt_tracef` probes. Asm
   comments inside `/* ... */` keep listing names on purpose — they quote the
   original, not the C namespace.
5. **No name collisions** across modules — tnd names get `tnd_` prefix in the
   port; main names must not collide with existing `loc_*`/`sub_*` labels.
6. **`lifted/` is maintained source.** Never re-run `tools/lift.py` to apply
   renames — it overwrites manual semantic fixes. `rename.py` edits in place.
7. **Regeneration stays working.** `gen_port.py` reads `names.map`: fmap addr
   lookup translates new names back to old (`NAME2OLD`), tnd offsets and
   TRACE_PROCS translate forward.

## Workflow

```
read body → find evidence → verify callees/callers → add map line →
python3 tools/rename.py → rebuild + make check + tests/e2e.sh
```

Each rename makes neighboring bodies more readable — batch by subsystem
(timing, video pipeline, resource loader, menu flow, TANDYSND) rather than
walking addresses.

## Current coverage

184 names (177 main + 7 overlay). Verified clusters:

- **battle loop** (`post_mission_loop` callees): `battle_key_check`
  (speed `word_26582` + fire `byte_28cc0` + weapon `byte_29711` keys),
  `obj_tick_all` (`funcs_1892f[type]` dispatch over 0x22 slots),
  `obj_motion_tick`, `render_tick` (`word_265ae` cadence → `redraw_frame`),
  `spawn_tick`, `enemy_spawn`, `find_free_slot_b`, `obj_rec_clear`,
  `pick_spawn_xy`, `slot_weight_sum`, `ai_param_fetch`, `target_pri_decay`,
  `sprite_reset`, `grid_cell_mark`, `action_dispatch` (`jpt_16ab9[byte_2967a]`),
  `unit_draw_walk`/`unit_draw_emit` (8-slot ordered draw-list emit),
  `hud_weapon_update`, `fx_overlay_fill`
- **POD screen**: `pod_cursor_move` (dir deltas + clamps, action→SF),
  `pod_hit_test` (37-rect hit list ds:0xBC9B → `word_28c83`, bx class 0/1/2 →
  `funcs_159e9` dispatch), `sprite_blit_flagged`; `hud_setup` sets
  `word_1d920=5`
- **descent/ground**: `descent_steer` (`byte_2a9ca` gates input-steer vs
  drift anim), `ground_map_draw` (40×25 tile paint), `tile_blit`,
  `load_palette_b`
- **overlay far-jmp slots**: `farjmp_ptr_10231/36/3b/40/45/4f` — `jmp far`
  through `load_overlay`-patched dwords (cf `patched_trampoline`,
  `farcall_ptr_a9e`)
- **gfx mode**: `gfx_mode_menu`, `clear_text_buf`, `draw_text_table`,
  `set_bda_equip_video`, `resize_prog_block`
- **display pipeline**: `vblank_wait`, `compose_frame`, `compose_frame_cond`,
  `flip_frame`, `compose_flip`, `adapter_compose_flip`, `draw_tile_compose`,
  `blit_tile_mcga`, `build_mcga_lut`, `build_tile_tables`, `tilemap_compose`,
  `mcga_dirty_update`, `video_bufs_init`, `video_bufs_setup`, `set_video_mode`,
  `clear_backbuf`, `clear_flipbuf`, `clear_drawbuf`, `clear_all_bufs`
- **timing/input**: `timer_tick_isr` (int1c countdown ISR), `frame_wait`,
  `poll_input`, `delay_3_ticks`, `delay_ticks`, `frame_compose_flip`,
  `read_key`, `key_poll`, `kb_flush`, `wait_btn_release`, `toggle_mode_flag`,
  `rand_next` (BIOS-seeded Galois LFSR, 681 call sites)
- **resources**: `load_res_pair`, `select_resource`/`_b`/`_c`/`_d` (four
  cache slots 1d008/0a/0c/0e), `load_resource`, `decompress_res` (LZW),
  `res_file_read`, `seek_res_entry`, `open_res_file`, `close_res_file`,
  `res_file_error`, `res_load_fail`, `load_overlay`, `load_res_0a`
- **map/objects**: `map_cell_read`, `map_probe_xy`, `map_cell_write`,
  `map_init`, `obj_overlap_test`, `update_obj_max`, `init_obj_table`,
  `obj_lookup_word`, `clear_8_slots`, `sprite_param_set`
- **flow**: `game_main` (loc_14760 — real top-level loop), `intro_animation`,
  `draw_tilemap_frame`, `input_device_menu`, `show_device_menu`,
  `draw_menu_screen`, `menu_screen_run`, `title_screen_once`, `clear_table`,
  `mission_run`, `mission_init_dispatch`, `call_mission_fn`,
  `load_stage_parms`, `dispatch_phase`, `reset_counters`, `reset_run_state`,
  `reset_tbl_ptr`, `hud_setup`, `copy_draw_params`, `redraw`-adjacent helpers
- **DOS glue**: `farcall_ptr_a9e`, `patched_trampoline`, `speaker_beep`
- **TANDYSND**: `tnd_timer_isr` (IRQ0 /3 chain), `tnd_seq_tick`,
  `tnd_install_timer`, `tnd_uninstall_timer`, `tnd_install_timer_2`,
  `tnd_timer_isr_chain`, `tnd_module_init`

The `word_1d934` adapter index drives every `jpt_*` display dispatch:
compose (jpt_11999), flip (jpt_11a25), clear (jpt_10b57/10ad3/10b34),
mode set (jpt_113e3), buf setup (jpt_126d5/1039d), compose+flip (jpt_11c4e).
Per-adapter cases are internal `loc_*` labels — no separate procs to name.
