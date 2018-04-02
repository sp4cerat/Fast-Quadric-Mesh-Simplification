self.addEventListener('message', function(e) {
    const file = e.data.blob;
    const percentage = e.data.percentage;
    const simplify_name = e.data.simplify_name;

    simplify(file, percentage, simplify_name);
}, false);

var Module = {
    'print': function(text) {
        self.postMessage({"log":text});
    }
};

self.importScripts("a.out.js");

function simplify(file, percentage, simplify_name) {

    console.log("in worker process", file, percentage, simplify_name);

    var filename = file.name;
    var fr = new FileReader();
    fr.readAsArrayBuffer(file);

    fr. onloadend = function (e) {
        var data = new Uint8Array(fr.result);
        try {
            Module.FS_stat(filename);
        } catch (err) {
            Module.FS_createDataFile(".", filename, data, true, true);
        }

        Module.ccall("simplify", // c function name
                undefined, // return
                ["string", "number", "string"], // param
                [filename, percentage, simplify_name]
        );
        let out_bin = Module.FS_readFile(simplify_name);
        // sla should work for binary stl
        let file = new Blob([out_bin], {type: 'application/sla'});
        self.postMessage({"blob":file});
    }
}
