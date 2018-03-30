self.addEventListener('message', function(e) {
    var file = e.data.blob;
    var percentage = e.data.percentage;
    simplify(file, percentage);
}, false);

var Module = {
    'print': function(text) {
        self.postMessage({"log":text});
    }
};

self.importScripts("a.out.js");

function simplify(file, percentage) {

    console.log("in worker process", file, percentage);

    var filename = file.name;
    var fr = new FileReader();
    fr.readAsArrayBuffer(file);

    fr. onloadend = function (e) {
        var data = new Uint8Array(fr.result);
        Module['FS_createDataFile'](".", filename, data, true, true);
        Module.ccall("simplify", // c function name
                undefined, // return
                ["string", "number"], // param
                [filename, percentage]
        );
        const simplify_filename = 'simplify.stl';
        let out_bin = Module['FS_readFile'](simplify_filename);
        // sla should work for binary stl
        let file = new Blob([out_bin], {type: 'application/sla'});
        self.postMessage({"blob":file, "original_name":filename});
    }
}
