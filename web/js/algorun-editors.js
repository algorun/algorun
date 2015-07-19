var i_editor = ace.edit("input_editor");
i_editor.setTheme("ace/theme/xcode");
i_editor.getSession().setTabSize(4);
i_editor.getSession().setUseSoftTabs(true);
document.getElementById('input_editor').style.fontSize='15px';
i_editor.getSession().setMode("ace/mode/json");

var o_editor = ace.edit("output_editor");
o_editor.setTheme("ace/theme/twilight");
o_editor.getSession().setTabSize(4);
o_editor.getSession().setUseSoftTabs(true);
document.getElementById('output_editor').style.fontSize='15px';
o_editor.getSession().setMode("ace/mode/json");
o_editor.setReadOnly(true);