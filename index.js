var fs = require('fs')
	, path = require('path')
	, request = require('request')
	, instagram = require('instagram')
	, q = require('q');

var config = {
	client_id: '32b0b8023d24433ca39ef42efba6e1b2'
	, client_secret: '7dfc63116732489685cafd9d74bb727f'
	, path: 'bin/data/img/'
}

var instaClient = instagram.createClient(config.client_id, config.client_secret);


function downloadImage(uri, filename, callback) {
	request.head(uri, function(err, res, body){
    request(uri).pipe(fs.createWriteStream(filename)).on('close', callback);
  });
}

function requestImages() {
	instaClient.tags.media('selfie', function (tags, err) {

		if(err) {
			throw err;
			return;
		}

		tags.forEach(function(item) {
			var id = item.id
				, p = config.path + id +'.jpg'
				, url;

			if(item.images && item.images.standard_resolution) {

				url = item.images.standard_resolution.url;

				// only dl if file doesn't already exist
				if(!fs.existsSync(p)) {
					downloadImage(url, p, function() {
						console.log(p + ' downloaded');
					});
				}
			}
			
		});

	});
}

requestImages();

