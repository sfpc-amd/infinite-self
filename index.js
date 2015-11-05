var fs = require('fs')
	, path = require('path')
	, request = require('request')
	, instagram = require('instagram')
	, q = require('q');

var config = {
	client_id: '32b0b8023d24433ca39ef42efba6e1b2'
	, client_secret: '7dfc63116732489685cafd9d74bb727f'
	, path: 'bin/data/img/'
	, maxImages: 1000
	, timeout: 120000
}

var instaClient = instagram.createClient(config.client_id, config.client_secret);


function downloadImage(uri, filename, callback) {
	request.head(uri, function(err, res, body){
    request(uri).pipe(fs.createWriteStream(filename)).on('close', callback);
  });
}

function requestImages() {
	instaClient.tags.media('selfie', function (tags, err) {

		var dfds = [];

		if(err) {
			throw err;
			return;
		}

		tags.forEach(function(item) {
			var id = item.id
				, p = config.path + id +'.jpg'
				, url
				, dfd = q.defer();

			if(item.images && item.images.standard_resolution) {

				url = item.images.standard_resolution.url;

				// only dl if file doesn't already exist
				if(!fs.existsSync(p)) {
					dfds.push(dfd.promise);
					downloadImage(url, p, function() {
						dfd.resolve(p);
						console.log(p + ' downloaded');
					});
				}
			}		
		});

		q.allSettled(dfds).spread(imagesDownloaded);

	});
}


function imagesDownloaded() {
	console.log('images downloaded');
	
	var files = fs.readdirSync(config.path);

	console.log(files.length);
	if(files.length > config.maxImages) {
		files.sort(function(a, b) {
			var tA = fs.statSync(config.path + a).mtime.getTime()
				, tB = fs.statSync(config.path + b).mtime.getTime();
	  	return tA - tB;                   
		}).reverse();

		for(var i = files.length-1; i > config.maxImages; i--) {
			console.log("delete file "+i+' '+ files[i]);
			fs.unlinkSync(config.path + files[i]);
		}
	}

}

requestImages();
setInterval(requestImages, config.timeout);

