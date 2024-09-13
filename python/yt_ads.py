#!/usr/bin/env python3


from selenium import webdriver
import selenium.common.exceptions
from PIL import Image
from io import BytesIO
from time import sleep
from imagededup.methods.hashing import PHash, Hashing
import os
import numpy as np
phash = PHash()


'''
To find Youtube's advert video_id:

function path_to_key(object, path_so_far, key){
	for(const [k,v] of Object.entries(object)){
		if (k === key) {
			console.log(v().video_id, path_so_far);
			return;
		}
		if (v && typeof v === 'object'){
			if (k === "_yt_player"){
				// Avoid unnecessary self-reflection
				continue;
			}
			if (["parent","self","top","frames","window","$jscomp"].includes(k)){
				// _yt_player.v.parent.parent. ... === _yt_player.v
				// yt_player.v.$jscomp.global.$jscomp.global == _yt_player.v
				// This is just obfuscation BS by Youtube
				continue;
			}
			if (k === "document")
				continue;
			if (path_so_far.length < 80)
				path_to_key(v, path_so_far+"."+k, key);
		}
	}
}
Error.stackTraceLimit = 1000;
path_to_key(_yt_player, '_yt_player', 'getVideoData');
'''


#CURRENTLY_PLAYING_VIDEO_ID:str = "return _yt_player.v.ytPubsubPubsubInstance.subscriptions_[24].app.Y.ga.ra.api.u.getVideoData().video_id;"
# Firefox only
# WARNING: I am not sure this path works all the time. It seems to, but 


def process_yt_vid_ads(driver, min_hamming_distance:int):
	while True:
		try:
			vid = driver.find_element_by_class_name("html5-main-video")
			break
		except selenium.common.exceptions.NoSuchElementException:
			sleep(0.1)
	i:int = 0
	# Hide video overlays: link to advertiser's website, advertiser's channel, and duration of advert:
	# One of these cannot be deleted because it contains the ad-duration-remaining nodes
	# Also hide ytd-mealbar-promo-renderer - the YouTube Premium popup
	driver.execute_script("""
		for(const s of ["ytp-ad-module","ytp-chrome-bottom","ytp-chrome-top","ytd-mealbar-promo-renderer"]){
			const todel = document.getElementsByClassName(s)[0];
			if(todel!==undefined)
				todel.style.display = "none";
		}
	""")
	clicked:bool = False
	while True:
		sleep(1) # Lazy way of ensuring the advert is loaded. TODO: Watch the 'ad-showing' (and related) class
		try:
			x = driver.find_element_by_css_selector("span[id^='ad-duration-remaining:']")
		except selenium.common.exceptions.NoSuchElementException:
			# There is not an advert currently playing.
			if clicked:
				return
			try:
				driver.execute_script('document.getElementsByClassName("ytp-play-button")[0].click()')
				#driver.find_element_by_class_name("ytp-play-button").click() # Play the video itself. This often triggers a sneaky advert by Youtube.
			except selenium.common.exceptions.JavascriptException: #selenium.common.exceptions.ElementNotInteractableException:
				print("  ERROR: Cannot click on video to kickstart ad")
				return
			print("  Clicked")
			sleep(1)
			clicked = True
			continue
		ad_video_id:str = None
		try:
			ad_video_id = driver.execute_script("""
				try {
					return _yt_player.v.ytPubsubPubsubInstance.o[3].app.Ha.A.u.getVideoData().video_id;
				} catch(e) {
					return _yt_player.v.ytPubsubPubsubInstance.subscriptions_[24].app.Y.ga.ra.api.u.getVideoData().video_id;
				}
			""")
		except selenium.common.exceptions.JavascriptException as e:
			print(f"  {e.__class__.__name__}: {e}")
			print("  Either no advert, or literally anything else because I haven't dug into the Youtube obfuscated API enough to figure it out")
			
			# find_element_by_class_name("html5-video-info-panel-content").find_element_by_tag_name("span").text.split(" / ")[0]
		outdir:str = f"/tmp/selenium/{ad_video_id}"
		countfile:str = f"{outdir}/_count"
		try:
			os.mkdir(outdir)
		except FileExistsError:
			# Advert has already been encountered
			n:int = None
			with open(countfile, "r") as f:
				n = int(f) + 1
			with open(countfile, "w+") as f:
				f.write(str(n))
		img1_hash = None
		while True:
			# Iterate through each frame, and if it is distinct enough from previous frames, save a screenshot
			try:
				x.text
			except selenium.common.exceptions.StaleElementReferenceException:
				# Element is no longer attached to the DOM
				break
			img = Image.open(BytesIO(vid.screenshot_as_png))
			img2_hash = phash.encode_image(image_array=np.array(img))
			save_img:bool = (img1_hash is None)
			if not save_img:
				save_img = (Hashing.hamming_distance(img1_hash, img2_hash) >= min_hamming_distance)
			img1_hash = img2_hash
			if save_img:
				vid.screenshot(f"{outdir}/{i}.png")
			i += 1
			open(countfile, "w").write("1")
			sleep(0.1)


def goto_next_video_of_different_channel(driver, prev_viewed_channels:list):
	for node in driver.find_elements_by_class_name("ytd-watch-next-secondary-results-renderer"):
		try:
			channel:str = node.find_element_by_id("byline-container").text
		except selenium.common.exceptions.NoSuchElementException:
			continue
		except selenium.common.exceptions.StaleElementReferenceException:
			# Why would this happen?
			continue
		if channel in prev_viewed_channels:
			continue
		href:str = node.find_element_by_tag_name('a').get_attribute('href')
		if not href.startswith("https://www.youtube.com/watch?v="):
			continue
		prev_viewed_channels.append(channel)
		driver.get(href)
		return
	# Could not find any related video not by a previously scraped channel
	goto_recommended_yt_vid(driver)


def goto_recommended_yt_vid(driver):
	driver.get("https://youtube.com/")
	try:
		driver.find_element_by_id("remind-me-later-button").click() # Hide button that takes up space
	except selenium.common.exceptions.NoSuchElementException:
		pass
	driver.find_element_by_xpath('//*[@id="thumbnail"]').click()


if __name__ == "__main__":
	import argparse
	
	parser = argparse.ArgumentParser()
	parser.add_argument("--chrome", default=False, action="store_true")
	parser.add_argument("--w", type=int, default=689, help="Width of the browser (not the video itself)")
	parser.add_argument("--h", type=int, default=548, help="Height of the browser (not the video itself)")
	parser.add_argument("--min-hamming-distance", type=int, default=20)
	args = parser.parse_args()
	
	driver = (webdriver.Chrome if args.chrome else webdriver.Firefox)(executable_path="/usr/bin/geckodriver")
	driver.set_window_size(args.w, args.h)
	prev_viewed_channels:list = []
	
	goto_recommended_yt_vid(driver)
	while True:
		process_yt_vid_ads(driver, args.min_hamming_distance)
		goto_next_video_of_different_channel(driver, prev_viewed_channels)
		print("  Sleeping 5s")
		sleep(5)
