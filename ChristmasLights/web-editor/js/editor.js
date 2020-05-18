$(document).ready(function() {
  var editor = ace.edit("editor");
  editor.setTheme("ace/theme/monokai");
  editor.getSession().setMode("ace/mode/python");
  editor.getSession().setTabSize(2);
  $.post("/cgi-bin/read.py", "", function(data) {
    editor.setValue(data);
  });

  $("#save").click(function(){
    $.post("/cgi-bin/write.py", editor.getValue());
  });
});
