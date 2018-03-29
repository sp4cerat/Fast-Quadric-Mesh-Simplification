self.addEventListener('message', function(e) {
    var file = e.data;
    simplify(file);
}, false);

var Module = {
    'print': function(text) {
        self.postMessage(text);
    }
};

self.importScripts("a.out.js");

function simplify(file) {

    console.log("in worker process", file);

    var filename = file.name;
    var fr = new FileReader();
    fr.readAsArrayBuffer(file);

    fr. onloadend = function (e) {
        var data = new Uint8Array(fr.result);
        Module['FS_createDataFile'](".", filename, data, true, true);
        Module.ccall("simplify", // c function name
                undefined, // return
                ["string", "number"], // param
                [filename, 0.01]
        );
        const simplify_filename = 'simplify.stl';
        let out_bin = Module['FS_readFile'](simplify_filename);
        console.log(out_bin)
    }
}
