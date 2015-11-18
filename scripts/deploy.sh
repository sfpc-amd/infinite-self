#/bin/bash

#create backup
ssh SFPC@capture.fios-router.home 'bash -s' < ./scripts/remote_backup.sh

#sync latest image data
rsync -avz ../sharedData/ SFPC@capture.fios-router.home:oFApps/sharedData/ --exclude '.DS_Store'

#sync latest app files
rsync -avz --delete bin/ SFPC@capture.fios-router.home:oFApps/infiniteSelf/bin/ --exclude '.DS_Store'