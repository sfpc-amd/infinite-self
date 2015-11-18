#/bin/bash

#create backup
ssh SFPC@capture.fios-router.home 'bash -s' < ./scripts/remote_backup.sh

#sync latest files
rsync -avz --delete bin/ SFPC@capture.fios-router.home:oFApps/infiniteSelf/ --exclude '.DS_Store'