# Discogs Tagger

foo_discogs is component for [foobar2000](http://www.foobar2000.org/) that allows you to tag your
music with information pulled from the [Discogs](http://www.discogs.com) database. foo_discogs aims to
expose all information from the Discogs database and to be highly customizable by allowing users to 
write tags based on formatting strings.

## Use Case

Discogs is based on the following principles, which are reflected in foo_discogs:

* The smallest unit is a release.
* It is important to identify the exact version of the release.

Therefore, this component is not greatly useful for tagging individual songs and it is not useful for quick, 
automatic tagging without much concern about which version of the release you have. It requires quite a bit 
of manual effort on the part of the user.

# Getting Started

## Download

Get the component from [the official foobar2000 components page](http://www.foobar2000.org/components/view/foo_discogs).

## OAuth and TLS 1.2

Discogs now requires OAuth authentication and TLS 1.2 support for anyone to access its API. 

To authenticate via OAuth, you'll need to have a Discogs account. When you first open the Find Release 
dialog without having configured OAuth, you will automatically be prompted to do so. Alternatively, you 
can open the Configuration dialog and look at the OAuth tab. Follow the instructions on the OAuth Identity tab.
Note that your system clock will need to be configured correctly for OAuth to work.

If you are using Windows XP or Windows 7, or for some other operating systems, you may need to enable TLS 1.2
support in your OS.

## Basic Usage

#### 1. Select the files in your foobar2000 playlist that you would like to tag.

Right click. Select Tagging > Discogs > Write Tags...

It is  recommended that you configure a hotkey within foobar2000 if you will be doing this a lot.

#### 2. Use the Find Release dialog to find the exact release you are tagging.

foo_discogs will automatically try to search for the artist and filter for the release name. (This can be
configured on the Searching tab of the Configuration dialog.) Found artists are listed on the left hand side.
Found releases for the selected artist are listed on the right hand side.

If there is a master release on the right hand side, you'll need to click on it to show the individual releases.
If you need to, you can double click on an artist or release to open their Discogs page in your browser.

Once you've found the exact release you want to tag, select it or enter its unique release id in the bottom
right corner, then click "Next>>".

#### 3. Use the Match Tracks dialog to match the Discogs tracklist to your files.

foo_discogs will try to automatically match your tracks as per the settings on the Matching tab of the
Configuration dialog. Matching based on track durations will only work if the tracks have different durations 
and the durations entered on Discogs match the actual file durations (durations entered on Discogs from the 
artwork can be inaccurate).

If needed, you can move tracks up and down or remove them to get your files matched.

Once your files are matched to the Discogs tracklist, continue on to "Write tags" or "Preview tags" if you'd like
to preview them before writing.

#### 4. Use the Preview Tags dialog to perfect your tag mappings.

If you want to customize the tags you're writing, and not just use the defaults that foo_discogs comes with,
you'll want to use the Preview Tags dialog to preview the tags before they are written.

If "Normal" view is selected, all tags while be written. "Difference" will only show tags that will be modified. 
"Debug" prints arrays with square brackets so that you can see exactly what kind of result is being generated by
the tag formatting string.

Tags can be customized on the "Edit tag mappings" dialog. Here, tags can be added or removed. They can also be
disabled or enabled only for writing or updating, or for both. More on updating tags later.

You may notice that some tags are greyed out and cannot be disabled or deleted. These are core tags that are
required by foo_discogs for functionality provided by the component. If you really insist on deleting them, you can
create another tag with the same name further down in the list and a blank formatting string. This will have the
effect of deleting the tag.

#### 5. Write tags to file.

Once you're satisified with your track matching and tag formatting string configurations, the final step
is the write tags to file. Several additional options around writing tags are provided on the Tagging tab of the 
Configuration dialog.

## Updating Tags

Once tags have been written, you may way to go back at a later date and update them. Since the release id has
already been identified and stored in the tags, this process can be done more simply and, in many cases, without
requiring any manual effort.

#### 1. Select all files in your foobar2000 playlist that you would like to update tags for.

Right click. Select Tagging > Discogs > Update Tags...

#### 2. Use the Match Tracks dialog to match the Discogs tracklist to your files wherever automatic matching failed.

foo_discogs attempts to automatically match tracks when updating tags. For any releases that this fails, you 
will be prompted to manually match tags later on, before tags are written.

## Saving Artwork

foo_discogs provides the ability to download images of the release or artist from Discogs and same them locally.
This can be configured in the Artwork tab of the Configuration dialog. It is not possible to automatically embed
artwork in the files.

## Viewing Pages on Discogs

Once you've tagged your files, foo_discogs provides shortcuts to open your browser to the release or artist page.
For example, to view the release page, with your file selected:

Right click. Select Tagging > Discogs > View Release Page

It is  recommended that you configure a hotkey within foobar2000 if you will be doing this a lot.

# Advanced Topics

## Arrays and Array Functions

To provide great flexibility in tagging, and to follow the spirit of foobar2000, foo_discogs built upon the 
string formatting syntax and functions built in to foobar2000. However, the built-in formatting strings were not
sophisticated enough for us as they lacked the concept of an array, which is essential as many items returned 
from Discogs are in array format and multiple values can be written to a single tag. Therefore, foo_discogs 
extended the built-in formatting strings by adding arrays.

To understand how we do arrays, as well as the functions that foo_discogs provides for working with arrays, refer
to the "Syntax help" document which is linked from the Configuration dialog within the component. To see how 
arrays are being returned from your tag formatting strings, use "Debug" view on the Preview Tags dialog.

## Data Objects and Fields

Refer to the "Syntax help" document, which is linked from the Configuration dialog within the component, for a 
list of objects and their data fields. There is also a list of root objects where fields must start from, and 
the sub-objects that each object type has.

## Discogs API and Caching

foo_discogs only loads the data that it needs when generating tags. Data is cached once loaded. The cache size 
can be observed and it can be cleared in the Caching tab of the Configuration dialog.

Note that the Discogs API also does caching of its own, so if you've recently modified a release in the Discogs
database you might have to wait awhile before foo_discogs sees the changes via the Discogs API.

# About

## Authors

* Michael Pujos (aka bubbleguuum) (up until v1.32)
* zoomorph (v1.33+)

## Community

Check out the age-old thread about foo_discogs on 
[hydrogenaudio forums](http://www.hydrogenaud.io/forums/index.php?showtopic=50523) for help or discussion 
with other users.

## Development

Bug or feature request? File it on the Issues tracker on BitBucket.

foo_discogs is built with Microsoft Visual Studio. You are welcome to add a feature and submit a pull request.

### Dependencies

The following dependencies will have to be linked for you to be able to compile foo_discogs:

* [foobar2000 SDK](http://www.foobar2000.org/SDK)
* [Jansson](http://www.digip.org/jansson/)
* [liboauthcpp](https://github.com/sirikata/liboauthcpp)
* [zlib](http://www.zlib.net/)

### Packaging

1. In Windows File Explorer, select files to package: foo_discogs.dll, foo_discogs_help.html, titleformat_help.css
2. Right click on selected files, Send To > Compressed (zipped) folder
3. Rename the created zip file as "foo_discogs.fb2k-component"

The fb2k-component file can then be loaded in foobar2000 as a component or uploaded to the official components site.