if (window.File && window.FileReader && window.FileList && window.Blob) {
  function handleFileSelect(evt) {
    var files = evt.target.files; // FileList object
    var r = new FileReader();
    f = files[0];
        r.onload = function(e) { 
	    var contents = e.target.result;
        i_editor.setValue(contents);
      }
      r.readAsText(f);
  }
  document.getElementById('file-input').addEventListener('change', handleFileSelect, false);
}